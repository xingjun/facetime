#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include<time.h>
#include <stdlib.h>
#include "datadecoder.h"
#include "stream.h"
#include "buffer.h"
#include "log.h"
#include "crypt.h"
#include "socket_manager.h"

static size_t decoder_get_buflen(char* buf);
static int decoder_state_pack(FDHandler* _fdHandler,char* buf,size_t buflen);
static int decoder_lobby_pack(FDHandler* _fdHandler,char* buf,size_t buflen);
static int decoder_client_pack(FDHandler* _fdHandler,char* buf,size_t buflen);
static int decoder_client_token(FDHandler* _fdHandler,BinaryStream * stream, int * idx, char* skey);


int decoder_unpack(FDHandler* _fdHandler)
{
              DataBuf *recvdb = &(_fdHandler->recvBuf);           
		char *buf = recvdb->m_buf;
		char *ptr = buf;
		size_t pos = recvdb->m_pos;		
		while(1)// ptr < (buf+pos) )
		{
			if( (size_t)(buf + pos - ptr) <= BINARY_PACKLEN_LEN)
				break;
			size_t buflen = decoder_get_buflen(ptr);
			if(buflen == 0)
			{
			       recvdb->m_pos=0;
				RL_ERROR("get buflen error");
				return -1;
			}
			if( (ptr + buflen) > (buf + pos) )
				break;

                    switch(_fdHandler->nFDType)
			{
       	         case FD_TYPE_LISTEN_SERVER:break;
	                case FD_TYPE_LISTEN_CLIENT:break;
	                case FD_TYPE_CLIENT_STATE:{
			     if(decoder_state_pack(_fdHandler,ptr,buflen) != 0)
			     {
				recvdb->m_pos=0;
				return -1;
			      }		
		          }
		            break;
	               case FD_TYPE_CLIENT_CLIENT:{
			      if(decoder_client_pack(_fdHandler,ptr,buflen) != 0)
			     {
				recvdb->m_pos=0;
				return -1;
			      }
	               	}
		            break;
	               case FD_TYPE_CLIENT_LOBBY:{
				if(decoder_lobby_pack(_fdHandler,ptr,buflen) != 0)
			     {
				recvdb->m_pos=0;
				return -1;
			      }
	               	}
		             break;
	               default:
				   	break;
		       }			
			
			if(_fdHandler->bIsColsed)
			{
				recvdb->m_pos=0;
				return 0;
			}
			ptr += buflen;
		}
		if( (buf + pos - ptr) == 0)
			recvdb->m_pos=0;
		else
			buffer_data_copy(recvdb,ptr,buf + pos -ptr);		
		return 0;
}

static int decoder_state_pack(FDHandler* _fdHandler,char* buf,size_t buflen)
{
 return 0;
}

static int decoder_lobby_pack(FDHandler* _fdHandler,char* buf,size_t buflen)
{ 

       BinaryStream read;
	stream_init(&read,buf,buflen);	
	short cmd;
	if(!stream_read_short(&read,&cmd))
	{
		RL_ERROR("read cmd error");
		return 0;
	}
	//RL_MSG("recv cmd=%d",cmd);
	int seq;
	if(!stream_read_int(&read,&seq))
	{
		RL_ERROR("read seq error");
		return 0;
	}
	//RL_MSG("recv seq=%d",seq);	
  switch(cmd)
	{
	case LOBBY_RELAY_LOGIN:
		{	
		   int idx = 0;
               
	if(!stream_read_int(&read,&idx))
	{
		RL_ERROR("read idx error");
		return 0;
	}	   
	_fdHandler->idx=idx;
		}
		break;
	case LOBBY_RELAY_SPEAK_ALL_L2R2C:
		{		
			int lobbyid;
			if(!stream_read_int(&read,&lobbyid))
			{
				RL_ERROR("cmd=LOBBY_RELAY_SPERK_LOBBY_L2R2C, read toidx error");
				return -1;
			}	
				int fromidx;
			if(!stream_read_int(&read,&fromidx))
			{
				RL_ERROR("cmd=LOBBY_RELAY_SPEAK_ALL_L2R2C, read toidx error");
				return -1;
			}	
			char szContext[300]={0};
                    size_t strContextlen;			
			stream_read(&read,szContext,sizeof(szContext),&strContextlen);			
			char * temp=buffer_get_pub_char();
			memset(temp,0,BLOCK_LENGTH);
		   BinaryStream write;		   
		   stream_init(&write, temp,BLOCK_LENGTH);			  
		   stream_write_char(&write,'[');		   
		   stream_write_short(&write,(short)RELAY_CMD_CHAT_PUBLIC_ALL_C2R2C);		
		   stream_write_int(&write,0);	
		   stream_write_int(&write,fromidx);
		   stream_write(&write,szContext,strContextlen);		   
		   stream_write_flush(&write);		   
		   RL_MSG("socket_manager_relay_all_client:fromidx:%d,context:%s",fromidx,szContext);
		   socket_manager_relay_all_client(&write);
		}
		break;
			case LOBBY_RELAY_SPERK_LOBBY_L2R2C:
		{			   
				int lobbyid;
			if(!stream_read_int(&read,&lobbyid))
			{
				RL_ERROR("cmd=LOBBY_RELAY_SPERK_LOBBY_L2R2C, read toidx error");
				return -1;
			}	
			int fromidx;
			if(!stream_read_int(&read,&fromidx))
			{
				RL_ERROR("cmd=LOBBY_RELAY_SPERK_LOBBY_L2R2C, read toidx error");
				return -1;
			}
			char szContext[300]={0};
                    size_t strContextlen;			
			stream_read(&read,szContext,sizeof(szContext),&strContextlen);			
			char * temp=buffer_get_pub_char();
			memset(temp,0,BLOCK_LENGTH);
		   BinaryStream write;		   
		   stream_init(&write, temp,BLOCK_LENGTH);			  
		   stream_write_char(&write,'[');		   
		   stream_write_short(&write,(short)RELAY_CMD_CHAT_PUBLIC_LOBBY_C2R2C);		
		   stream_write_int(&write,0);	
		   stream_write_int(&write,fromidx);
		   stream_write(&write,szContext,strContextlen);		   
		   stream_write_flush(&write);		   
		   RL_MSG("socket_manager_relay_lobby_client:fromidx:%d,lobbyidx:%d,context:%s",fromidx,lobbyid,szContext);
		   socket_manager_relay_lobby_client(lobbyid,&write);
		}
		break;
		default:
			{
				RL_ERROR("read cmd error,cmd = %d",cmd);
				return 0;
			}
			break;
	}
 return 0;
}

static int decoder_client_pack(FDHandler* _fdHandler,char* buf,size_t buflen)
{
      const char *pinbuf = buf;
	size_t inbuflen = buflen;
	char outbuf[65535];
		
	if(_fdHandler->bIsToken)
	{
		//解密
		int outbuflen = sizeof(outbuf);		
		if(buf[2] == TEAFLAG)
		{
			if(!StreamDecrypt(buf,buflen,outbuf,&outbuflen,_fdHandler->sessionkey))
			{
				RL_ERROR("cann't decrypt,ip = %s sessionkey=%s",socket_get_ip(_fdHandler),socket_get_port(_fdHandler),_fdHandler->sessionkey);
				return -1;
			}
			pinbuf = outbuf;
			inbuflen = outbuflen;
		}
		else if(buf[2] == COMPRESSFLAG)
		{
			if(!StreamUnCompress(buf,buflen,outbuf,&outbuflen))
			{
				RL_ERROR("cann't uncompress,ip = %s",socket_get_ip(_fdHandler));
				return -1;
			}
			pinbuf = outbuf;
			inbuflen = outbuflen;
		}
		else
		{
			RL_ERROR("buf[2] flag error");
			return -1;
		}
	}

       BinaryStream read;
	stream_init(&read,(char*)pinbuf,inbuflen);
	if(_fdHandler->bIsToken)
	{
		char c;
		if(!stream_read_char(&read,&c))
		{
			RL_ERROR("read c error");
			return -1;
		}
		//RL_MSG("recv c=%d",c);		
	}
	short cmd;
	if(!stream_read_short(&read,&cmd))
	{
		RL_ERROR("read cmd error");
		return 0;
	}
	//RL_MSG("recv cmd=%d",cmd);
	int seq;
	if(!stream_read_int(&read,&seq))
	{
		RL_ERROR("read seq error");
		return 0;
	}
	//RL_MSG("recv seq=%d",seq);		
	switch(cmd)
	{
	case RELAY_CMD_TOKEN:
		{	
		   int idx = 0;
                char strsessionkey[16];                
                if(decoder_client_token(_fdHandler, &read, &idx, strsessionkey) == -1)
                {
                    RL_ERROR("decoder_client_pack RELAY_LOGIN token error");
                    return -1;
                }                
		  char strversion[12] = {0};
                size_t strversionlen;
                if(!stream_read(&read,strversion,sizeof(strversion),&strversionlen))
                {
                    RL_ERROR("read strversion error");
                    return -1;
                }
                float clientversion;
                clientversion = atof(strversion);
		   socket_client_login(_fdHandler, strsessionkey);
		   _fdHandler->idx=idx;
		   RL_MSG("%d token!",idx);
		   char sendbuf[65535];
		   BinaryStream write;
		   stream_init(&write, sendbuf,sizeof(sendbuf));			  
		   stream_write_char(&write,'[');		   
		   stream_write_short(&write,cmd);		
		   stream_write_int(&write,seq);
		   stream_write_int(&write,0);
		   stream_write(&write,strsessionkey,strlen(strsessionkey));		   
		   stream_write_flush(&write);		   
		   char tempsessionkey[16] = {0};
		   strncpy(tempsessionkey,FIRSTKEY,sizeof(tempsessionkey));
		   
		   socket_client_send_XTEABuf(_fdHandler,  &write,tempsessionkey);		   
		}
		break;
		case RELAY_CMD_KEEPALIVE:
		{	
			RL_MSG("%d keeplive!",_fdHandler->idx);
		}
		break;
		case RELAY_CMD_CHAT_PUBLIC_ALL_C2R2C://test
		{			   
			char szContext[300]={0};
                    size_t strContextlen;			
			if (!stream_read(&read,szContext,sizeof(szContext),&strContextlen))
			{
				RL_ERROR("cmd=RELAY_CMD_CHAT_PUBLIC_ALL_C2R2C, content  error");
				return -1;
			}
			
			char * temp=buffer_get_pub_char();
			memset(temp,0,BLOCK_LENGTH);
		   BinaryStream write;		   
		   stream_init(&write, temp,BLOCK_LENGTH);			  
		   stream_write_char(&write,'[');		   
		   stream_write_short(&write,(short)RELAY_CMD_CHAT_PUBLIC_ALL_C2R2C);		
		   stream_write_int(&write,0);	
		   stream_write_int(&write,_fdHandler->idx);
		   stream_write(&write,szContext,strContextlen);		   
		   stream_write_flush(&write);		   
		   socket_manager_relay_all_client(&write);
		}
		break;
		case RELAY_CMD_CHAT_PRIVATE_C2R2C:
		{			
			int toidx;
			if(!stream_read_int(&read,&toidx))
			{
				RL_ERROR("cmd=RELAY_CMD_CHAT_PRIVATE_C2R2C, read toidx error");
				return -1;
			}	
			char content[512];
			size_t contentlen=0;
			if (!stream_read(&read,content, sizeof(content), &contentlen))
			{
				RL_ERROR("cmd=RELAY_CMD_CHAT_PRIVATE_C2R2C, content  error");
				return -1;
			}
			content[contentlen] = 0;
			RL_MSG("recv a RELAY_CMD_CHAT_PRIVATE_C2R2C,fromidx = %d,toidx = %d, context = %s", _fdHandler->idx,toidx, content);

			char * temp=buffer_get_pub_char();
			memset(temp,0,BLOCK_LENGTH);
		   BinaryStream write;		   
		   stream_init(&write, temp,BLOCK_LENGTH);			  
		   stream_write_char(&write,'[');		   
		   stream_write_short(&write,(short)RELAY_CMD_CHAT_PRIVATE_C2R2C);		
		   stream_write_int(&write,0);	
		   stream_write_int(&write,_fdHandler->idx);
		   stream_write(&write,content,contentlen);		   
		   stream_write_flush(&write);		   
		   
		   //socket_manager_relay_all_client(&write);
		   FDHandler* tempP=socket_manager_get_client(toidx);
		   if(tempP!=NULL)
		   	socket_client_send_buf(tempP,&write);		   
		}
		break;	
		
			case RELAY_CMD_REPORT_LOBBYID:
		{			   
			int lobbyid;
			if(!stream_read_int(&read,&lobbyid))
			{
				RL_ERROR("cmd=RELAY_CMD_REPORT_LOBBYID, read lobbyid error");
				return -1;
			}
			RL_MSG("RELAY_CMD_REPORT_LOBBYID:lobbyid:%d,idx:%d",lobbyid,_fdHandler->idx);
			
		_fdHandler->lobbyid=lobbyid;
		}
		break;
		default:
			{
				RL_ERROR("read cmd error,cmd = %d",cmd);
				return 0;
			}
			break;
	}
	return 0;

}

static int decoder_client_token(FDHandler* _fdHandler,BinaryStream * stream, int * idx, char* skey)
{
   char token[256];
    size_t tokenlen;
    if(!stream_read(stream,token,sizeof(token),&tokenlen))
    {
        RL_ERROR("read token error");
        return -1;
    }
    char userinfo[128] = {0};
    RL_MSG("serverkey:%s,tokenlen:%d",g_rlConfig.serverkey,tokenlen);
    int retlen = XTEADecrypt(token,tokenlen,userinfo,sizeof(userinfo),g_rlConfig.serverkey);

    if(retlen == 0)//token解析失败
    {
        RL_ERROR("decrypt token error,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    RL_DEBUG("token = %s",userinfo);
    char seps[] = "&";
    char *stridx = strtok(userinfo,seps);
    if(stridx == NULL)
    {
        RL_ERROR("idx error in token,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    *idx = atoi(stridx);  

    char *strid = strtok(NULL,seps);
    if(strid == NULL)
    {
        RL_ERROR("id error in token,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    char *strtime = strtok(NULL,seps);
    if(strtime == NULL)
    {
        RL_ERROR("time error in token,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    time_t tokentime = atoi(strtime);
    if(time(NULL) - tokentime > 20*60)//120秒内必须来取2010.3.11 20*60
    {
        RL_ERROR("token time out,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    char *strsessionkey = strtok(NULL,seps);
    if(strsessionkey == NULL)
    {
        RL_ERROR("sessionkey error in token,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    int strsessionkeylen = strlen(strsessionkey);
    if(strsessionkeylen > 16)//要改
    {
        RL_ERROR("sessionkey error in token,ip = %s",socket_get_ip(_fdHandler));
        return -1;
    }
    strncpy(skey,strsessionkey,16);
    _fdHandler->idx=*idx;		
    RL_MSG("token:IDX:%d,ID:%s,sessionkey:%s",*idx,strid,strsessionkey);
  return 0;
}
static size_t decoder_get_buflen(char* buf)
{
   uint16_t len;
   memcpy(&len,buf,sizeof(len));
   len = ntohs(len);
   return (size_t)len;
}



