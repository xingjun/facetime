#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <sys/time.h>
#include "config/config.h"
#include "timer.h"
#include "stream.h"
#include "buffer.h"
#define CONNECT_TIMEOUT 3
#define WRITE_TIMEOUT 3

extern RLConfig g_rlConfig;
enum EN_FD_TYPE
{
    FD_TYPE_LISTEN_SERVER =1,
    FD_TYPE_LISTEN_CLIENT =2,
    FD_TYPE_CLIENT_STATE =3,
    FD_TYPE_CLIENT_CLIENT =4,
    FD_TYPE_CLIENT_LOBBY =5,
};
typedef struct _FDHandler
{
     int nFd;
     short regEevntType;   
     int nFDType;
     timer readTimerOut;
     timer writeTimerOut;
     timer timeout;
     bool bHasRead;
     bool bHasWrite;
     bool bIsConnected;
     bool bIsColsed;
     bool bIsToken;     
     DataBuf sendBuf;
     DataBuf recvBuf;
     char sessionkey[16]; 
     int idx;
     int lobbyid;//for client
}FDHandler;

	
int socket_init(FDHandler* _newFD);
int socket_listen(FDHandler* _fdHandler,char* ip,short port);
int socket_connect(FDHandler * _fdHandler,char* ip,short port);

int socket_read_dispatch(FDHandler * _fdHandler);
int socket_write_dispatch(FDHandler * _fdHandler);

int socket_add_read_timeout(FDHandler * _fdHandler,struct timeval *_tm);
int socket_add_write_timeout(FDHandler * _fdHandler,struct timeval *_tm);
int socket_add_timeout(FDHandler * _fdHandler,struct timeval *_tm);
int socket_del_read_timeout(FDHandler * _fdHandler);
int socket_del_write_timeout(FDHandler * _fdHandler);
int socket_del_timeout(FDHandler * _fdHandler);

char* socket_get_ip(FDHandler* _fdHandler);
short socket_get_port(FDHandler* _fdHandler);

bool socket_client_send_XTEABuf(FDHandler * _fdHandler,BinaryStream *write,char sessionkey[16]);
bool socket_client_send_buf(FDHandler * _fdHandler,BinaryStream *write);

void socket_client_login(FDHandler* _fdHandler,char* sessionkey);
#endif
#ifdef  __cplusplus
}
#endif

