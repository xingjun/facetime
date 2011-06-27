#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include<unistd.h> 

/**/
int _bind(int fd, const struct sockaddr* addr, int addrlen)
{
  return bind(fd, addr, addrlen);
}

int _listen(int fd,int size)
{
   return listen(fd,size);
}

int _connect(int fd,struct sockaddr* addr, int addrlen)
{
  return connect(fd, addr, addrlen);
}

void  _close(int fd)
{
   close(fd);
}

int  _accept(int fd,const struct sockaddr* addr, socklen_t* addrlen)
{
   return accept(fd,(struct sockaddr*)addr,addrlen);   
}


ssize_t _send(int fd,const void *msg, int len, int flags)
{
  return send(fd,msg,len,flags);
}

ssize_t _recv(int fd,char* buf,size_t buflen,size_t len)
{
 return recv(fd,buf,buflen,len);
}
