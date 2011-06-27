#ifndef _DATADECODER_H_
#define _DATADECODER_H_

enum RELAYCMD
{
    RELAY_CMD_TOKEN = 3000,                   //token验证						C=>S,S=>C		ReserveData		NULL
	RELAY_CMD_KEEPALIVE,                     //心跳包						C=>S
	RELAY_CMD_CHAT_PUBLIC_ALL_C2R2C,      //聊天包	
	RELAY_CMD_CHAT_PUBLIC_LOBBY_C2R2C,      //聊天包	
	RELAY_CMD_CHAT_PRIVATE_C2R2C,
	RELAY_CMD_REPORT_LOBBYID,
};
enum LOBBY_RELAY
{
       LOBBY_RELAY_LOGIN =0,
	//LOBBY_RELAY_CHAT_L2R2C ,
	LOBBY_RELAY_SPEAK_ALL_L2R2C,
	LOBBY_RELAY_SPERK_LOBBY_L2R2C ,
};

#define FIRSTKEY "9158xingguang"


#include "socket.h"
int decoder_unpack(FDHandler* _fdHandler);
#endif
