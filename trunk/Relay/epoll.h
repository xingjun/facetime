#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _EPOLL_H_
#define _EPOLL_H_
#include "socket.h"

#define INITIAL_NCLIENTS 10000
#define ADD_NCLIENTS 500
#define MAX_NCLIENTS 35000
#define RET_NCLIENTS 500

int epoll_init();
int epoll_add_read(FDHandler* _fdHandler,struct timeval *_tm);
int epoll_add_write(FDHandler* _fdHandler,struct timeval *_tm);
int epoll_add(FDHandler* _fdHandler,struct timeval *_tm);
int epoll_del_read(FDHandler*  _fdHandler);
int epoll_del_write(FDHandler* _fdHandler);
int epoll_del(FDHandler* _fdHandler);
int epoll_run();
int epoll_stop();

#endif
#ifdef  __cplusplus
}
#endif
