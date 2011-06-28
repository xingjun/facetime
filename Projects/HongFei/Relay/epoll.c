#include <stdint.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "epoll.h"
#include "log.h"
#include "socket.h"
#include "timer.h"


typedef struct _epollop {		
	int nEpfd;
	FDHandler** pFDArray;  	
	int nFDs;		    
}epollop;
static struct _epollop s_epollop;

static int epoll_recalc();
int  epoll_init()
{
	int nEpfd;	
	if ((nEpfd = epoll_create(32000)) == -1) {
		if (errno != ENOSYS)
			RL_WARN("epoll_create");
		return (-1);
	}
	s_epollop.nEpfd = nEpfd;	
	s_epollop.pFDArray=  (FDHandler**)malloc(INITIAL_NCLIENTS * sizeof(FDHandler*));
	if (s_epollop.pFDArray == NULL)
	{
		return -1;
	}
	int i;
	for(i=0;i<INITIAL_NCLIENTS;i++)
	{
		s_epollop.pFDArray[i]=0;
	}
	s_epollop.nFDs= INITIAL_NCLIENTS;
	RL_MSG("Epoll init:fds=%d",s_epollop.nFDs);
	return 0;
}

int epoll_add_read(FDHandler* _fdHandler,struct timeval *_tm)
{
	int ret;
	int op;
	int flag = 0;
	__uint32_t evtype = EPOLLIN;
	
	int fd;
	fd = _fdHandler->nFd;
	if (fd >= s_epollop.nFDs) {	
		if (epoll_recalc() == -1)
			return (-1);
	}
	FDHandler *p  =s_epollop.pFDArray[fd];
	if(p == NULL)
	{ 
		op = EPOLL_CTL_ADD;
	}
	else
	{
		flag=p->regEevntType;
		op = EPOLL_CTL_MOD;			
		if(flag & EPOLLOUT)
			evtype = EPOLLIN | EPOLLOUT;
	}
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = evtype;
	
	if((ret = epoll_ctl(s_epollop.nEpfd,op,fd,&ev)) < 0)
	{
		RL_ERROR("epoll_ctl < 0 %s",strerror(errno));
		return -1;
	}
	_fdHandler->regEevntType|= EPOLLIN;
	RL_MSG("register:fd:%d,op:%d,event:%d",fd,op,_fdHandler->regEevntType);
	if(s_epollop.pFDArray)
		s_epollop.pFDArray[fd]=_fdHandler;
	if (_tm != NULL)
		{
		       socket_add_read_timeout(_fdHandler,_tm);
		}	
	return (0);
}

int epoll_add_write(FDHandler* _fdHandler,struct timeval *_tm)
{  
    int ret;
	int op;
	int flag = 0;
	__uint32_t evtype = EPOLLOUT;
		
    int fd;
    fd = _fdHandler->nFd;      
	if (fd >= s_epollop.nFDs) {		
		if (epoll_recalc() == -1)
			return (-1);
	}
	FDHandler *p  =s_epollop.pFDArray[fd];
	if(p == NULL)
	{ 
		op = EPOLL_CTL_ADD;	
	}
	else
	{
		flag=p->regEevntType;
		op = EPOLL_CTL_MOD;			
		if(flag & EPOLLIN)
			evtype = EPOLLOUT | EPOLLIN;
	}
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = evtype;
	if((ret = epoll_ctl(s_epollop.nEpfd,op,fd,&ev)) < 0)
	{
		RL_ERRORX("epoll_ctl < 0 fd:%d %s",fd,strerror(errno));
		return -1;
	}
	_fdHandler->regEevntType|= EPOLLOUT;
	RL_MSG("register:fd:%d,op:%d,event:%d",fd,op,_fdHandler->regEevntType);
	if(s_epollop.pFDArray)
		s_epollop.pFDArray[fd]=_fdHandler;

	if (_tm!=NULL)
		{
		       socket_add_write_timeout(_fdHandler,_tm);
		}
	return (0);

}

int epoll_add(FDHandler* _fdHandler,struct timeval *_tm)
{
    int ret;
	int op;	
	__uint32_t evtype = EPOLLOUT | EPOLLIN;
		
	int fd;
	fd = _fdHandler->nFd;     
	if (fd >= s_epollop.nFDs) {		
		if (epoll_recalc() == -1)
			return (-1);
	}	

	FDHandler *p  =s_epollop.pFDArray[fd];
	if(p == NULL)
	{ 
		op = EPOLL_CTL_ADD;			
	}
	else
	{		      
		op = EPOLL_CTL_MOD;								
	}
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = evtype;
	RL_MSG("register:fd:%d,op:%d,event:%d",fd,op,_fdHandler->regEevntType);
	if((ret = epoll_ctl(s_epollop.nEpfd,op,fd,&ev)) < 0)
	{
		RL_ERROR("epoll_ctl < 0 %s",strerror(errno));
		return -1;
	}
	_fdHandler->regEevntType = evtype;
	if(s_epollop.pFDArray)
		s_epollop.pFDArray[fd]=_fdHandler;

	if (_tm!=NULL)
		{
		       socket_add_read_timeout(_fdHandler,_tm);
			socket_add_write_timeout(_fdHandler,_tm);
		}
	return (0);
    
}

int epoll_del_read(FDHandler*  _fdHandler)
{
             int op;
		int flag = 0;
		__uint32_t evtype;
		int fd = _fdHandler->nFd;
		if(fd>=s_epollop.nFDs)
			return 0;
		FDHandler *p  =s_epollop.pFDArray[fd];
		if(p == NULL)
		{ 
			op = EPOLL_CTL_DEL;
		}
		else
		{
		       flag=p->regEevntType;		       
			if(flag & EPOLLOUT)
			{
				op = EPOLL_CTL_MOD;
				evtype = EPOLLOUT;
			}
			else
			{
				op = EPOLL_CTL_DEL;
			}
		}
		
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = evtype;
		
		if(epoll_ctl(s_epollop.nEpfd,op,fd,&ev) < 0)
		{
			RL_ERROR("epoll_ctl < 0");
			return -1;
		}           
		if((_fdHandler->regEevntType&= (~EPOLLIN)) == 0)
		{
		       if(s_epollop.pFDArray)
			   	s_epollop.pFDArray[fd]=0;
		}
		 socket_del_read_timeout(_fdHandler);
		return 0;
    
}

int epoll_del_write(FDHandler* _fdHandler)
{
             int op;
		int flag = 0;
		__uint32_t evtype;
		int fd = _fdHandler->nFd;
		if(fd>=s_epollop.nFDs)
			return 0;
		FDHandler *p  =s_epollop.pFDArray[fd];
		if(p == NULL)
		{ 
			op = EPOLL_CTL_DEL;
		}
		else
		{
		       flag=p->regEevntType;		       
			if(flag & EPOLLIN)
			{
				op = EPOLL_CTL_MOD;
				evtype = EPOLLIN;
			}
			else
			{
				op = EPOLL_CTL_DEL;
			}
		}		
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = evtype;
		
		if(epoll_ctl(s_epollop.nEpfd,op,fd,&ev) < 0)
		{
			RL_ERROR("epoll_ctl < 0");
			return -1;
		}

		if((_fdHandler->regEevntType&= (~EPOLLOUT)) == 0)
		{
		       if(s_epollop.pFDArray)
			   	s_epollop.pFDArray[fd]=0;
		}
		 socket_del_write_timeout(_fdHandler);
		return 0;
    
}

int epoll_del(FDHandler* _fdHandler)
{             			
		int fd = _fdHandler->nFd;		
		struct epoll_event ev;
		ev.data.fd = fd;			
		if(epoll_ctl(s_epollop.nEpfd,EPOLL_CTL_DEL,fd,&ev) < 0)
		{
			RL_ERRORX("epoll_ctl < 0 fd:%d",fd);
			return -1;
		}		        
	       if(s_epollop.pFDArray)
		   s_epollop.pFDArray[fd]=0;   
		socket_del_read_timeout(_fdHandler);
		socket_del_write_timeout(_fdHandler);
		 return 0;
}

int epoll_run()
{
	struct epoll_event events[RET_NCLIENTS];//ÒÔÇ°ÊÇ
	int i, res, sockfd;
	RL_MSG("Epoll run");
	for ( ; ; ) {
	res = epoll_wait(s_epollop.nEpfd, events, RET_NCLIENTS, 100);
	if (res < 0) {		
		RL_ERRORX("epoll_wait");			
		break;
	}
	for (i = 0; i < res; i++) {
		if ((events[i].events & (EPOLLERR|EPOLLHUP))
					&& (events[i].events & (EPOLLIN|EPOLLOUT)) == 0)
				{				       
					events[i].events |= EPOLLIN|EPOLLOUT;
				}
		
		if(events[i].events & EPOLLIN)
				{
					
					if ( (sockfd = events[i].data.fd) < 0) 
						continue;
					
					if(sockfd>=s_epollop.nFDs)
						continue;
					FDHandler  *pHandler= s_epollop.pFDArray[sockfd];					
					if(!pHandler)
					{
						RL_ERROR("pHandler == NULL");
						continue;
					}
					socket_read_dispatch(pHandler);                                
					
				}
		if(events[i].events & EPOLLOUT)
				{
					
					if( (sockfd = events[i].data.fd) < 0)
						continue;
					//int fd=pHandler->GetFD();
					if(sockfd>=s_epollop.nFDs)
						continue;
					
					FDHandler  *pHandler= s_epollop.pFDArray[sockfd];
					if(!pHandler)
					{
						RL_ERROR("pHandler == NULL");
						continue;
					}
					socket_write_dispatch(pHandler);   
			}
		}
	       timer_scan();
		}
	return (0);
}

int epoll_stop()
{
   RL_MSG("Epoll stop!");
   if(s_epollop.pFDArray)
   	{
   	memset(s_epollop.pFDArray, 0, sizeof(FDHandler*)*s_epollop.nFDs);
   	//free(s_epollop.pFDArray);   
   	}
   return (0);
}

static int
epoll_recalc()
{
	if(s_epollop.nFDs<MAX_NCLIENTS)
       {
           FDHandler** fdArry;
	    int nfds=s_epollop.nFDs+ADD_NCLIENTS;
	    fdArry = (FDHandler**)realloc(s_epollop.pFDArray, nfds * sizeof(FDHandler*));
		  if (fdArry == NULL) {
			RL_WARN("realloc");
			return (-1);
		  	}	 
		s_epollop.pFDArray = fdArry;
		memset(fdArry + s_epollop.nFDs, 0,ADD_NCLIENTS * sizeof(FDHandler*));
		 s_epollop.nFDs=nfds;	
		 return (0);
		}
	return (0);
}


