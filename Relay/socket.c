#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "socket.h"
#include "log.h"
#include "epoll.h"
#include "socket_sub.h"
#include "datadecoder.h"
#include "buffer.h"
#include "socket_manager.h"
#include "crypt.h"

#define MAXBUFSIZETEA 256



static void socket_close(FDHandler * _fdHandler);
static int socket_init_buf(FDHandler* _newFD);
static int socket_uninit_buf(FDHandler* _newFD);
static int socket_set_block(FDHandler* _fdHandler,bool isBlock);

static int socket_read_timeout_dispatch(void * p);
static int socket_write_timeout_dispatch(void * p);
static int socket_timeout_dispatch(void * p);
static int socket_client_read_timeout(FDHandler* _fdHandler);
static int socket_client_write_timeout(FDHandler* _fdHandler);


/*static int socket_client_server_read(FDHandler* _fdHandler);
static int socket_client_client_read(FDHandler* _fdHandler);
static int socket_client_lobby_read(FDHandler* _fdHandler);*/

static int socket_client_server_write(FDHandler* _fdHandler);
static int socket_client_client_write(FDHandler* _fdHandler);
static int socket_client_lobby_write(FDHandler* _fdHandler);
static int socket_write(FDHandler* _fdHandler);

static int socket_server_accpept(FDHandler* _fdHandler);
static int socket_client_accpept(FDHandler* _fdHandler);
static int socket_client_read(FDHandler* _fdHandler);

static int socket_client_server_connected(FDHandler* _fdHandler);
static int socket_report_state_login(FDHandler* _fdHandler);

static bool socket_client_send_TEAbuf(FDHandler * _fdHandler,BinaryStream *write);
static bool socket_client_send_Compressbuf(FDHandler * _fdHandler,BinaryStream *write);
static bool socket_add_buf(FDHandler* _fdHandler,char* buf,size_t buflen);

int socket_init(FDHandler* _newFD)
{   
  _newFD->nFd = -1; 
   _newFD->regEevntType=0;
   _newFD->bIsConnected=false;       
     timer_init_(&(_newFD->readTimerOut));
     timer_init_(&(_newFD->writeTimerOut));
     timer_init_(&(_newFD->timeout));
     _newFD->bHasRead=false;
     _newFD->bHasWrite=false;
     _newFD->bIsConnected=false;
     _newFD->bIsColsed=true;
     _newFD->bIsToken=false;
     _newFD->idx=0;
     _newFD->lobbyid=-1;
   return 0;
}
int socket_listen(FDHandler* _fdHandler,char* ip,short port)
{
	       if(socket_init(_fdHandler)==-1)
			{
			   RL_ERRORX("init listen socket error!");
			   return -1;
			}
		   socket_init_buf(_fdHandler);
		   
		  if(_fdHandler->nFd==-1)
			_fdHandler->nFd=socket(AF_INET,SOCK_STREAM,0);	
		struct  sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);	
		int i = 1;
		if(setsockopt(_fdHandler->nFd, SOL_SOCKET, SO_REUSEADDR, (void*)&i, sizeof(int)) == -1)
		{			
			RL_ERRORX("setsocketopt error");
			return -1;
		}
              //RL_MSG("bind fd:%d,ip:%s,port:%d",_fdHandler->nFd,_fdHandler->ip,_fdHandler->port);
		if (_bind(_fdHandler->nFd, (struct sockaddr*)(&addr), sizeof(struct sockaddr))!= 0)		
		{
			RL_ERRORX("bind error fd = %d errno = %d,error info:%s", _fdHandler->nFd, errno,strerror(errno));		
			return -1;
		}
		if(_listen(_fdHandler->nFd,500) != 0)
		{
			RL_ERRORX("listen error");	
			return -1;
		}			
             if(epoll_add_read(_fdHandler,NULL)!=0)
			 	{
			RL_ERRORX("register read error");
			return -1;
		}
		return 0;
}
int socket_connect(FDHandler * _fdHandler,char* ip,short port)
{
  if(_fdHandler->bIsConnected)
			return 0;             
		struct timeval to;
			to.tv_sec=CONNECT_TIMEOUT;
			to.tv_usec=0;
             struct  sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);				
		if(socket_init(_fdHandler)!=0)
		{
			socket_add_timeout(_fdHandler,&to);
			return -1;
		}
		socket_init_buf(_fdHandler);
		if(_fdHandler->nFd==-1)
			_fdHandler->nFd=socket(AF_INET,SOCK_STREAM,0);
		if(socket_set_block(_fdHandler,true) != 0)
		{			
			socket_add_timeout(_fdHandler,&to);
			return -1;
		}
		int ret = _connect(_fdHandler->nFd,(struct sockaddr*)&addr,sizeof(struct sockaddr));		
		if(ret != 0 && errno != EINPROGRESS)
		{
		       RL_ERRORX("connect error");
			socket_close(_fdHandler);		
			socket_add_timeout(_fdHandler,&to);
			return -1;
		}		
		socket_del_timeout(_fdHandler);		
		if(epoll_add_write(_fdHandler,&to) != 0)
		{
			RL_MSG("RegisterWrite error");
			return -1;
		}		
		return 0; 
}

int socket_read_dispatch(FDHandler * _fdHandler)
{   
    RL_MSG("socket_read_dispatch type:%d,fd:%d",_fdHandler->nFDType,_fdHandler->nFd);
   _fdHandler->bHasRead=true;
    switch(_fdHandler->nFDType){
       	case FD_TYPE_LISTEN_SERVER:{
		      socket_server_accpept(_fdHandler);}
		break;
	case FD_TYPE_LISTEN_CLIENT:{
		      socket_client_accpept(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_STATE:{
			socket_client_read(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_CLIENT:{	
			socket_client_read(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_LOBBY:{	
			socket_client_read(_fdHandler);}
		break;
	default:			
		break;    
}
   
   return (0);
}
int socket_write_dispatch(FDHandler * _fdHandler)
{
  RL_MSG("socket_write_dispatch type:%d,fd:%d",_fdHandler->nFDType,_fdHandler->nFd);
  _fdHandler->bHasWrite=true;
   switch(_fdHandler->nFDType){
       	case FD_TYPE_LISTEN_SERVER:
		break;
	case FD_TYPE_LISTEN_CLIENT:
		break;
	case FD_TYPE_CLIENT_STATE:{
				if(!_fdHandler->bIsConnected)
					socket_client_server_connected(_fdHandler);					
       			socket_client_server_write(_fdHandler);									
		}
		break;
	case FD_TYPE_CLIENT_CLIENT:{	
			socket_client_client_write(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_LOBBY:{	
			socket_client_lobby_write(_fdHandler);}
	      break;		
	default:			
		break;}    
  return (0);
}

int socket_add_read_timeout(FDHandler * _fdHandler,struct timeval *_tm)
{
  
  timer* _timer=&_fdHandler->readTimerOut;
  _fdHandler->bHasRead=false;  
  _timer->regtime=*_tm;
  struct timeval nowtime;
  gettimeofday(&nowtime,NULL);
  Mtimeradd(&nowtime,_tm,&_timer->to);
  _timer->timver_arg=_fdHandler;
  _timer->timver_callback=socket_read_timeout_dispatch;
  timer_add(_timer);
  return 0;
}
int socket_add_write_timeout(FDHandler * _fdHandler,struct timeval *_tm)
{
  timer* _timer=&_fdHandler->writeTimerOut;
  _fdHandler->bHasRead=false;
  _timer->regtime=*_tm;
  struct timeval nowtime;
  gettimeofday(&nowtime,NULL);
  Mtimeradd(&nowtime,_tm,&(_timer->to));
  _timer->timver_arg=_fdHandler;
  _timer->timver_callback=socket_write_timeout_dispatch;
  timer_add(_timer);
 return 0;
}
int socket_add_timeout(FDHandler * _fdHandler,struct timeval *_tm)
	{
  timer* _timer=&_fdHandler->timeout;
  _fdHandler->bHasRead=false;
  _timer->regtime=*_tm;
  struct timeval nowtime;
  gettimeofday(&nowtime,NULL);
  Mtimeradd(&nowtime,_tm,&_timer->to);
  _timer->timver_arg=_fdHandler;
  _timer->timver_callback=socket_timeout_dispatch;
  timer_add(_timer);
 return 0;
}
int socket_del_read_timeout(FDHandler * _fdHandler)
{
	timer_del(&_fdHandler->readTimerOut);
	return 0;
}
int socket_del_write_timeout(FDHandler * _fdHandler)
{
       timer_del(&_fdHandler->writeTimerOut);
	return 0;
}
int socket_del_timeout(FDHandler * _fdHandler)
{
      timer_del(&_fdHandler->timeout);
	return 0;
}

char* socket_get_ip(FDHandler* _fdHandler)
{
   struct  sockaddr_in addr;
		socklen_t len = sizeof(struct sockaddr_in);
		getpeername(_fdHandler->nFd,(struct sockaddr*)&addr,&len);
		static char ip[32];
		strncpy(ip,inet_ntoa(addr.sin_addr),sizeof(ip));
		return ip;
}

short socket_get_port(FDHandler* _fdHandler)
{
   struct  sockaddr_in addr;
		socklen_t len = sizeof(struct sockaddr_in);
		getpeername(_fdHandler->nFd,(struct sockaddr*)&addr,&len);
		short port;
		port = ntohs(addr.sin_port);
		return port;

}

bool socket_client_send_XTEABuf(FDHandler * _fdHandler,BinaryStream *write,char sessionkey[16])
{
       char outbuf[65535];
	int outbuflen = sizeof(outbuf);
	if(StreamEncrypt_(write->ptr,write->cur-write->ptr,outbuf,&outbuflen,sessionkey,2))
	{
		//if(AddBuf(outbuf,outbuflen) != 0)
		if(socket_add_buf(_fdHandler,outbuf,outbuflen) != 0)
		{
			//Close();
			return false;
		}		
		return true;
	}
	return false;
  
}
bool socket_client_send_buf(FDHandler * _fdHandler,BinaryStream* write)
{
  if((write->cur-write->ptr) < MAXBUFSIZETEA)
			{
				return socket_client_send_TEAbuf(_fdHandler,write);
			}
			return socket_client_send_Compressbuf(_fdHandler,write);
  return 1;
}

void socket_client_login(FDHandler* _fdHandler,char* sessionkey)
{
   _fdHandler->bIsToken=true;
   strncpy(_fdHandler->sessionkey,sessionkey,sizeof(_fdHandler->sessionkey));
   RL_MSG("_fdHandler->sessionkey:%s",_fdHandler->sessionkey);
   struct timeval timeout;
   timeout.tv_sec=g_rlConfig.timeout2;
   timeout.tv_usec=0;
   epoll_add_read(_fdHandler, &timeout);	
}


/*static int socket_client_server_read(FDHandler* _fdHandler)
{
   
   return 0;
}
static int socket_client_client_read(FDHandler* _fdHandler)
{
   
   return 0;
}
static int socket_client_lobby_read(FDHandler* _fdHandler)
{
 
   return 0;
}*/



static int socket_server_accpept(FDHandler* _fdHandler)
{
    struct sockaddr_in clientaddr;
   socklen_t len = sizeof(clientaddr);
		int s = _accept(_fdHandler->nFd, (struct sockaddr*)(&clientaddr), &len);
		if (s == -1)
		{
			RL_ERRORX("::accept error");
			return -1;
		}
		RL_MSG("accept a server_client!");
		FDHandler* pFD=socket_manager_get_server();	
	if(pFD)
    {
      pFD->nFd=s;
	  pFD->bIsColsed=false;
	  pFD->nFDType=FD_TYPE_CLIENT_LOBBY;
	  socket_init_buf(pFD);	 	  
				if(epoll_add_read(pFD,NULL)!=0)
					{
					RL_ERROR("add read error");
					return -1;
					}			     
		}
	else 
		return -1;
		return 0;		
}
static int socket_client_accpept(FDHandler* _fdHandler)
{
  struct sockaddr_in clientaddr;
   socklen_t len = sizeof(clientaddr); 
   if(socket_manager_is_max())
   {
   RL_ERROR("client max!");
   return -1;
   	}
		int s = _accept(_fdHandler->nFd, (struct sockaddr*)(&clientaddr), &len); 		
		if (s == -1)
		{
			RL_ERRORX("::accept error");
			return -1;
		}
		RL_MSG("accept a client_client !");	
	FDHandler* pFD=socket_manager_get_fdhandler(s);	
	if(pFD)
    {
      pFD->nFd=s;
	  pFD->bIsColsed=false;
	  pFD->nFDType=FD_TYPE_CLIENT_CLIENT;
	  socket_init_buf(pFD);	 
	  socket_manager_client_increase();
	  struct timeval timeout_;
				timeout_.tv_sec = g_rlConfig.timeout;
				timeout_.tv_usec = 0;
				if(epoll_add_read(pFD,&timeout_)!=0)
					{
					RL_ERROR("add read error");
					return -1;
					}			     
		}
	else 
		return -1;
	return 0;	
}

static int socket_client_read(FDHandler* _fdHandler)
{
              char buf[16384] = {0};//16*1024
   ssize_t len = _recv(_fdHandler->nFd,buf,sizeof(buf),0);	
   RL_MSG("recv buflen:%d",len);
   if(len <= 0)
		{
			if(errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
			{				
				RL_ERRORX("recv errno = %d",errno);
				socket_close(_fdHandler);
				return -1;
			}
			else
			{
				RL_ERRORX("recv errno = %s",strerror(errno));				
				return -1;
			}
		}
   //拆包，粘包
             DataBuf *recvdb =&( _fdHandler->recvBuf);
             buffer_data_copy(recvdb, buf, len);
		decoder_unpack(_fdHandler); 
		return 0;
}

static  void socket_close(FDHandler * _fdHandler)
{
 epoll_del(_fdHandler);
  if(_fdHandler->nFDType==FD_TYPE_CLIENT_STATE)
  	{
  	   _fdHandler->bIsConnected=false;
	   struct timeval tm;
	   tm.tv_sec=CONNECT_TIMEOUT;
	   tm.tv_usec=0;
	   socket_add_timeout(_fdHandler,&tm);
  	}
  _close(_fdHandler->nFd);
  if(_fdHandler->nFDType==FD_TYPE_CLIENT_CLIENT)
  	{
  	  socket_manager_client_decrease();
  	}
  socket_init(_fdHandler);
  socket_uninit_buf(_fdHandler);  
}
static int socket_init_buf(FDHandler* _newFD)
{
  buffer_data_init(&(_newFD->sendBuf));
   buffer_data_init(&(_newFD->recvBuf));
   return 0;
}
 static int socket_uninit_buf(FDHandler* _newFD)
{
  buffer_data_init(&(_newFD->sendBuf));
   buffer_data_init(&(_newFD->recvBuf));
    return 0;
}
 static int socket_set_block(FDHandler* _fdHandler,bool isBlock)
{
	int flags = fcntl(_fdHandler->nFd, F_GETFL, 0);
		if (flags == -1)
		{
			RL_ERRORX("fcntl error");
			return -1;
		}
		if (isBlock)
			flags |= O_NONBLOCK;
		else
			flags &= ~O_NONBLOCK;
		if (fcntl(_fdHandler->nFd, F_SETFL, flags) != 0)
		{
			RL_ERRORX("fcntl error");
			return -1;
		}
		return 0;
}
 
static int socket_read_timeout_dispatch(void * p)
{
FDHandler* _fdHandler=(FDHandler*)p;
       switch(_fdHandler->nFDType){
       	case FD_TYPE_LISTEN_SERVER:{		
				socket_close(_fdHandler);}
		break;
	case FD_TYPE_LISTEN_CLIENT:{
				socket_close(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_STATE:{
				socket_client_read_timeout(_fdHandler);	}
		break;
	case FD_TYPE_CLIENT_CLIENT:{	
			socket_client_read_timeout(_fdHandler);	}
		break;
		case FD_TYPE_CLIENT_LOBBY:{	
			socket_client_read_timeout(_fdHandler);	}
		break;
	default:{
			socket_close(_fdHandler);}
		break;    
}

	return 0;
}
static int socket_write_timeout_dispatch(void * p)
{
FDHandler* _fdHandler=(FDHandler*)p;
       RL_MSG("socket_write_timeout_dispatch TYPE:%d",_fdHandler->nFDType);
	switch(_fdHandler->nFDType){
       	case FD_TYPE_LISTEN_SERVER:{		
				socket_close(_fdHandler);}
		break;
	case FD_TYPE_LISTEN_CLIENT:{
				socket_close(_fdHandler);}
		break;
	case FD_TYPE_CLIENT_STATE:{
				socket_client_write_timeout(_fdHandler);			}
		break;
	case FD_TYPE_CLIENT_CLIENT:{	
			socket_client_write_timeout(_fdHandler);		}
		break;
	case FD_TYPE_CLIENT_LOBBY:{	
			socket_client_write_timeout(_fdHandler);	}
		break;
	default:{
			socket_close(_fdHandler);		
		}
		break;
}
	return 0;
}
static int socket_timeout_dispatch(void * p)
{
FDHandler* _fdHandler=(FDHandler*)p;
    switch(_fdHandler->nFDType){
       case FD_TYPE_LISTEN_SERVER:break;
	case FD_TYPE_LISTEN_CLIENT:break;
	case FD_TYPE_CLIENT_STATE:{
				socket_connect(_fdHandler,socket_get_ip(_fdHandler),socket_get_port(_fdHandler));}
		break;
	case FD_TYPE_CLIENT_CLIENT:break;
	case FD_TYPE_CLIENT_LOBBY:break;
	default:		
		break;
		}
	return 0;
}
static int socket_client_read_timeout(FDHandler* _fdHandler)
{
 if (!_fdHandler->bHasRead)
			{ 
			      RL_MSG("socket_close fd:%d,min_idx:%d",_fdHandler->nFd,_fdHandler->readTimerOut.min_heap_idx);
				socket_close(_fdHandler);
			}
			else 
			{//重新插入一   				
				struct timeval nowtime;
				gettimeofday(&nowtime,NULL);
				Mtimeradd(&nowtime,&(_fdHandler->readTimerOut.regtime),&(_fdHandler->readTimerOut.to));
			       _fdHandler->bHasRead=false;	
				timer_add(&_fdHandler->readTimerOut);				
			}
			return 0;
}
static int socket_client_write_timeout(FDHandler* _fdHandler)
{
RL_MSG("socket_client_write_timeout:Type:%d,haswrite:%d,ispersister:%d",_fdHandler->nFDType,_fdHandler->bHasWrite,_fdHandler->writeTimerOut.bIsPersist);
if (!_fdHandler->bHasWrite)
			{
			      if(_fdHandler->nFDType==FD_TYPE_CLIENT_STATE)
			      	{
				if(_fdHandler->bIsConnected)
					{  socket_close(_fdHandler);}
				else {
					RL_MSG("reconnect ip:%s,port:%d",socket_get_ip(_fdHandler),socket_get_port(_fdHandler));
					socket_connect(_fdHandler,socket_get_ip(_fdHandler),socket_get_port(_fdHandler));}	
			      	}
				  else 
				  	{ 
				  	socket_close(_fdHandler);
				  	}
			}
			else 
			{//重新插入一    				
				struct timeval nowtime;
				gettimeofday(&nowtime,NULL);
				Mtimeradd(&nowtime,&(_fdHandler->writeTimerOut.regtime),&(_fdHandler->writeTimerOut.to));
			       _fdHandler->bHasRead=false;	
				timer_add(&_fdHandler->writeTimerOut);				
			}
			return 0;
}


static int socket_client_server_write(FDHandler* _fdHandler)
{    
   return socket_write(_fdHandler);
    
}
static int socket_client_client_write(FDHandler* _fdHandler)
{   
   return  socket_write(_fdHandler);
   
}
static int socket_client_lobby_write(FDHandler * _fdHandler)
{
   return socket_write(_fdHandler);
}
static int socket_write(FDHandler* _fdHandler)
{
   //size_t buflen = m_senddata.GetPos();
        DataBuf* senddata=&(_fdHandler->sendBuf);
        char* buf=senddata->m_buf;
		size_t buflen = senddata->m_pos;
		ssize_t len = _send(_fdHandler->nFd,buf,buflen,0);
		RL_MSG("fd:%d send buflen:%d",_fdHandler->nFd,len);
		if(len < 0)
		{
			if(errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
			{
			      RL_ERRORX("send error");
				//Close();
				socket_close(_fdHandler);
				return -1;
			}
			else
			{
				RL_ERRORX("send error");
				return -1;
			}
		}		
		if((size_t)len == buflen)
		{
			senddata->m_pos=0;
			epoll_del_write(_fdHandler);

			return 0;
		}
		else if((size_t)len < buflen)
		{
		    buffer_data_copy(senddata,buf+len,buflen-len);
		}
   return 0;
}

static int socket_client_server_connected(FDHandler* _fdHandler)
{
   RL_MSG("Connect ip:%s port:%d success!",socket_get_ip(_fdHandler),socket_get_port(_fdHandler));
   _fdHandler->bIsConnected=true;
   _fdHandler->bIsColsed=false;
   socket_del_timeout(_fdHandler);
   epoll_del_write(_fdHandler);  
   if(epoll_add_read(_fdHandler, (struct timeval*)NULL)!=0)
   	{
   	   RL_ERRORX("Register read error!");
   	   socket_close(_fdHandler);
   	}
   socket_report_state_login(_fdHandler);
   return 0;
}
static int socket_report_state_login(FDHandler* _fdHandler)
{
  return 0;
}

static bool socket_client_send_TEAbuf(FDHandler * _fdHandler,BinaryStream *write)
{
     char outbuf[65535];
	int outbuflen = sizeof(outbuf);
	if(StreamEncrypt(write->ptr,write->cur-write->ptr,outbuf,&outbuflen,_fdHandler->sessionkey))
	{
		
		       int ret;
			if((ret=socket_add_buf(_fdHandler ,outbuf,outbuflen)) != 0)
			{
				//Close();
				RL_ERROR("outbuflen=%d ret:%d",outbuflen,ret);
				return false;
			}
			return true;
		}	
		return false;	
}	
	 

static bool socket_client_send_Compressbuf(FDHandler * _fdHandler,BinaryStream *write)
{
 char outbuf[65535];
	int outbuflen = sizeof(outbuf);
	if(StreamCompress(write->ptr,write->cur-write->ptr,outbuf,&outbuflen))
	{
		
		       int ret;
			if((ret=socket_add_buf(_fdHandler,outbuf,outbuflen)) != 0)
			{
				//Close();
				RL_ERROR("outbuflen=%d ret:%d",outbuflen,ret);
				return false;
			}return true;
		}	
	return false;
}	
		 

static bool socket_add_buf(FDHandler* _fdHandler,char* buf,size_t buflen)
{
   if(_fdHandler->bIsColsed)
		{
			RL_ERROR("closed");
			return -2;
		}
      DataBuf * db=&(_fdHandler->sendBuf);
		if(buffer_data_copy(db,buf,buflen) != 0)
		{
			RL_ERROR("append send buf error");
			return -1;
		}
		struct timeval timeout;
		timeout.tv_sec=WRITE_TIMEOUT;
		timeout.tv_usec=0;
		if(epoll_add_write(_fdHandler,&timeout) != 0)
		{
			RL_ERROR("register write error");
			return -1;
		}
		return 0;
}
