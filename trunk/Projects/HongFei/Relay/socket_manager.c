#include <stdlib.h>
#include "socket_manager.h"
#include "epoll.h"
#include "socket.h"
#include "log.h"
#include "datadecoder.h"


static struct FDClient s_fdClient;
static struct FDServer s_fdServer;

int socket_manager_init()
{
    s_fdClient._clients=  (FDHandler*)malloc(INITIAL_NCLIENTS * sizeof(FDHandler));
	if (s_fdClient._clients == NULL)
	{
		return -1;
	}
	int i;
	for(i=0;i<INITIAL_NCLIENTS;i++)
	{
	       socket_init(&(s_fdClient._clients[i]));		
	}
	s_fdClient.nSize=INITIAL_NCLIENTS;
	s_fdClient.nClient=0;
	RL_MSG("socket_manager_init :fds=%d",s_fdClient.nSize);
	for(i=0;i<MAX_SERVER;i++)
		{
		socket_init(&(s_fdServer._servers[i]));
		}
	return 0;	
}
int socket_manager_increase()
{
      if(s_fdClient.nSize<MAX_NCLIENTS)
       {
           FDHandler* fdArry;
	    int nfds=s_fdClient.nSize+ADD_NCLIENTS;
	    fdArry =(FDHandler*) realloc(s_fdClient._clients, nfds * sizeof(FDHandler));
		  if (fdArry == NULL) {
			RL_WARN("realloc");
			return (-1);
		  	}	 
		s_fdClient._clients= fdArry;
		int i;
		for(i=s_fdClient.nSize;i<nfds;i++)
			{
			  socket_init(&(s_fdClient._clients[i]));	
			}

		 s_fdClient.nSize=nfds;	
		 return (0);
		}
	return (0);
}
FDHandler* socket_manager_get_fdhandler(int fd)
{
     
     while(fd>s_fdClient.nSize)
	 	if(!socket_manager_increase())
			break;
	 if(fd<s_fdClient.nSize)
	 	return &(s_fdClient._clients[fd]);
	 else 
	 	return 0;
}
int socket_manager_stop()
{
  return 0;
}

void socket_manager_client_increase()
{
   s_fdClient.nClient++;
}
void socket_manager_client_decrease()
{
	s_fdClient.nClient--;

}

bool socket_manager_is_max()
{
  return s_fdClient.nClient>MAX_NCLIENTS;
}

bool socket_manager_relay_all_client(BinaryStream* write)
{
       struct timeval timeout;
	timeout.tv_sec=WRITE_TIMEOUT;
	timeout.tv_usec=0;
       int i=0;
	int n=0;
	for(;i<s_fdClient.nSize;i++)
	{
		FDHandler *temp=&(s_fdClient._clients[i]);
		if(temp->bIsToken && temp->nFd!=-1)
	       {			  
	         socket_client_send_buf(temp,write);		 
		 n++;  
		}
		if(n==s_fdClient.nClient)
			break;			   
	}
  return true;
}

FDHandler* socket_manager_get_server()
{
     if(s_fdServer.nCurrent>MAX_SERVER)
	 	return NULL;
	 else 
	 {
	      // int nTemp=s_fdServer.nCurrent;
		 int i=s_fdServer.nCurrent+1;
		 bool isFind=false;
		 for(;i<MAX_SERVER;i++)
		 	if(s_fdServer._servers[i].nFd==-1)
		 		{
		 		isFind=true;
				s_fdServer.nCurrent=i;
				break;
		 		}
		if(!isFind)
			{
				s_fdServer.nCurrent=MAX_SERVER+1;
			}
	 	return &(s_fdServer._servers[s_fdServer.nCurrent]);
	 	}
	 return NULL;
}
FDHandler* socket_manager_get_client(int idx)
{ //可以考虑用hastable 
  int i=0;
  for(;i<s_fdClient.nSize;i++)
	{
		FDHandler *temp=&(s_fdClient._clients[i]);
		if(temp->idx==idx)
	       {	
	         return temp;			 
		}			   
	}
return NULL;
}

bool socket_manager_relay_lobby_client(int lobbyid,BinaryStream* write)
{//优化,,,
  int i=0;
  int n=0;
  for(;i<s_fdClient.nSize;i++)
	{
		FDHandler *temp=&(s_fdClient._clients[i]);
		if(temp->lobbyid==lobbyid)
	       {			  
	         socket_client_send_buf(temp,write);		 
		 n++;  
		}
		if(n==s_fdClient.nClient)
			break;			   
	}
  return 0;
}

