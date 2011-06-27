#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _RL_TIME_H_
#define _RL_TIME_H_
#include <sys/time.h>
#include <stdbool.h>

#define	evutil_timercmp(tvp, uvp, cmp)							\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?							\
	 ((tvp)->tv_usec cmp (uvp)->tv_usec) :						\
	 ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define	Mtimeradd(tvp, uvp, vvp)						\
	do {								\
	(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
	(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
	if ((vvp)->tv_usec >= 1000000) {			\
	(vvp)->tv_sec++;				\
	(vvp)->tv_usec -= 1000000;			\
	}							\
	} while (0)


	typedef struct timer_
		{
		   struct timeval to;//触发到时时间
                struct timeval regtime;	//时间间隔
                bool bIsPersist;
		   unsigned int min_heap_idx;
		   int (*timver_callback)(void *arg); /*回调函数arg对应于timver_arg*/
	         void *timver_arg;
		}timer;
	typedef struct  min_heap
	{
		timer** p;
		unsigned n, a;
	} min_heap_t;

	

	void timer_init();
	 
	void timer_stop();
	void timer_scan();
	
        void timer_init_(timer* _tm);
	int timer_add(timer* _tm);
	int timer_del(timer* _tm);

	
	   void           min_heap_ctor(min_heap_t* s);
	  void           min_heap_dtor(min_heap_t* s);
	  void           min_heap_elem_init(timer* e);
	  int            min_heap_elem_greater(timer *a, timer *b);
	  int            min_heap_empty(min_heap_t* s);
	  unsigned       min_heap_size(min_heap_t* s);
	   timer*  min_heap_top(min_heap_t* s);
	  int            min_heap_reserve(min_heap_t* s, unsigned n);
	  int            min_heap_push(min_heap_t* s,  timer* e);
	   timer*  min_heap_pop(min_heap_t* s);
	  int            min_heap_erase(min_heap_t* s,  timer* e);
	  void           min_heap_shift_up_(min_heap_t* s, unsigned hole_index,  timer* e);
	  void           min_heap_shift_down_(min_heap_t* s, unsigned hole_index,  timer* e);

#endif /* _MIN_HEAP_H_ */
#ifdef  __cplusplus
}
#endif

