#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_
#include <stdbool.h>
#include "socket.h"
#define MAX_SERVER 100
struct FDClient
{
   FDHandler* _clients;
   int nSize;
   int nClient;
};

struct FDServer
{
   FDHandler _servers[MAX_SERVER];
   int nCurrent;
};

int socket_manager_init();
int socket_manager_increase();
FDHandler* socket_manager_get_fdhandler(int fd);
void socket_manager_client_increase();
void socket_manager_client_decrease();
bool socket_manager_is_max();
int socket_manager_stop();
bool socket_manager_relay_all_client(BinaryStream* write);
bool socket_manager_relay_lobby_client(int lobbyid,BinaryStream* write);

FDHandler* socket_manager_get_server();
FDHandler* socket_manager_get_client(int idx);

#endif
