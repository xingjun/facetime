#include<stdlib.h>
#include "timer.h"
#include "log.h"
#include "socket.h"

      
	static struct min_heap s_timeHeap;
       void timer_init()
       	{       	
       	     min_heap_ctor(&s_timeHeap);
       	}
       void timer_init_(timer* _tm)
       	{
       	    min_heap_elem_init(_tm);
			_tm->bIsPersist=false;	 
       	}
	   
	    void timer_stop()
	   	{
	   	   min_heap_dtor(&s_timeHeap);
	   	}
	   

       int timer_add(timer* _tm)
       	{       	   
       	    if (_tm->min_heap_idx!=(unsigned int)-1)
			{	
			timer_del(_tm);
			}					
			min_heap_push(&s_timeHeap,(timer*)_tm);   
			//RL_MSG("add timer fd:%d min_idx:%d timer size:%d",((FDHandler*)_tm->timver_arg)->nFd,_tm->min_heap_idx,min_heap_size(&s_timeHeap));
			return 0;
       	}
	int timer_del(timer* _tm)
		{		  
		  min_heap_erase(&s_timeHeap,_tm);
		  return 0;
		}
	   void timer_scan()
	   	{
	   	   struct timeval nowtime;
		gettimeofday(&nowtime,NULL);
		
		timer* ev;
		while ((ev = min_heap_top(&s_timeHeap))) 
		{			
			if (evutil_timercmp(&ev->to, &nowtime, >))
				break;			
			//min_heap_erase(&s_timeHeap,ev);
			timer_del(ev);
			
			if(ev->bIsPersist)//如果是永久定时器，则需要重新插入
			{			      
			       struct timeval nowtime;
		   		gettimeofday(&nowtime,NULL);
				Mtimeradd(&nowtime,&(ev->regtime),&(ev->to));
				min_heap_push(&s_timeHeap,ev);				
			}
			ev->timver_callback(ev->timver_arg);
		}
	   	}
	int min_heap_elem_greater( timer *a,  timer *b)
	{
		return evutil_timercmp(&(a->to), &(b->to), >);	
	}

	void min_heap_ctor(min_heap_t* s) { s->p = 0; s->n = 0; s->a = 0; }
	void min_heap_dtor(min_heap_t* s) { if(s->p) free(s->p); }
	void min_heap_elem_init( timer* e) { e->min_heap_idx = -1; }
	int min_heap_empty(min_heap_t* s) { return 0u == s->n; }
	unsigned min_heap_size(min_heap_t* s) { return s->n; }
	timer* min_heap_top(min_heap_t* s) { return s->n ? *s->p : 0; }

	int min_heap_push(min_heap_t* s,  timer* e)
	{
		if(min_heap_reserve(s, s->n + 1))
			return -1;
		min_heap_shift_up_(s, s->n++, e);
		return 0;
	}

	timer* min_heap_pop(min_heap_t* s)
	{
		if(s->n)
		{
			timer* e = *s->p;
			min_heap_shift_down_(s, 0u, s->p[--s->n]);
			e->min_heap_idx = -1;
			return e;
		}
		return 0;
	}

	int min_heap_erase(min_heap_t* s,  timer* e)
	{
		if (s->n<1)
		{
			return -1;
		}		
		if(((unsigned int)-1) != e->min_heap_idx)
		{ 
		       
		       //RL_MSG("remove timer:min_idx:%d",e->min_heap_idx);
			
			timer *last = s->p[--s->n];
			unsigned parent = (e->min_heap_idx - 1) / 2;			
			if (e->min_heap_idx > 0 && min_heap_elem_greater(s->p[parent], last))
				min_heap_shift_up_(s, e->min_heap_idx, last);
			else
				min_heap_shift_down_(s, e->min_heap_idx, last);
			e->min_heap_idx = -1;
			return 0;
		}
		return -1;
	}

	int min_heap_reserve(min_heap_t* s, unsigned n)
	{
		if(s->a < n)
		{
			timer** p;
			unsigned a = s->a ? s->a * 2 : 8;
			if(a < n)
				a = n;
			if(!(p = ( timer**)realloc(s->p, a * sizeof *p)))
				return -1;
			s->p = p;
			s->a = a;
		}
		return 0;
	}

	void min_heap_shift_up_(min_heap_t* s, unsigned hole_index,  timer* e)
	{
		unsigned parent = (hole_index - 1) / 2;// 计算parent
		while(hole_index && min_heap_elem_greater(s->p[parent], e))
		{
			(s->p[hole_index] = s->p[parent])->min_heap_idx = hole_index;
			hole_index = parent;
			parent = (hole_index - 1) / 2;
		}
		(s->p[hole_index] = e)->min_heap_idx = hole_index;
	}

	void min_heap_shift_down_(min_heap_t* s, unsigned hole_index,  timer* e)
	{
		unsigned min_child = 2 * (hole_index + 1);
		while(min_child <= s->n)
		{
			min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child - 1]);
			if(!(min_heap_elem_greater(e, s->p[min_child])))
				break;
			(s->p[hole_index] = s->p[min_child])->min_heap_idx = hole_index;
			hole_index = min_child;
			min_child = 2 * (hole_index + 1);
		}
		min_heap_shift_up_(s, hole_index,  e);
	}

