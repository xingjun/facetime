#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "socket_manager.h"

static PublicBuf s_pubBuf;
static char s_pubChar[BLOCK_LENGTH];
int buffer_data_init(DataBuf* _dataBuf)
{
   _dataBuf->m_size=BLOCK_LENGTH;
   _dataBuf->m_buf= (char*)malloc(_dataBuf->m_size);
   return 0;
}


int buffer_data_copy(DataBuf* _dataBuf,const char *buf,size_t buflen)
{
   size_t tmppos = _dataBuf->m_pos +  buflen;
		if(  tmppos <= _dataBuf->m_size )
		{
			memcpy(_dataBuf->m_buf+_dataBuf->m_pos,buf,buflen);
			_dataBuf->m_pos = tmppos;
			return 0;
		}
		if( tmppos > _dataBuf->m_size && tmppos <= _dataBuf->m_maxsize)
		{
			char *tmpbuf =(char*)malloc(tmppos); 
			memcpy(tmpbuf,_dataBuf->m_buf,_dataBuf->m_pos);
			free(_dataBuf->m_buf);
			memcpy(tmpbuf + _dataBuf->m_pos,buf,buflen);

			_dataBuf->m_buf = tmpbuf;
			_dataBuf->m_pos = tmppos;
			_dataBuf->m_size = tmppos;
			return 0;
		}
		return -1;
   return 0;
}
int buffer_data_uninit(DataBuf* _dataBuf)
{
   if(_dataBuf->m_buf)
   	 free(_dataBuf->m_buf);
   return 0;
}

int buffer_pub_init()
{
   s_pubBuf.pFirstBuffer=(struct Buffer*)malloc(sizeof(Buffer));   
   Buffer* temp=s_pubBuf.pFirstBuffer;
   temp->iSize=BLOCK_NUM;
   temp->pcData=(char*)malloc(temp->iSize*BLOCK_LENGTH);
   temp->pNext=NULL;
  
   s_pubBuf.pFirstBlock= (struct Block*)malloc(sizeof(Block));
  Block* pb=s_pubBuf.pFirstBlock;
   for (int i = 1; i <temp->iSize; ++ i)
   {
      pb->pNext = (struct Block*)malloc(sizeof(Block));
      pb->iLength= 0;
      pb = pb->pNext;
   }
   pb->pNext = s_pubBuf.pFirstBlock;
   s_pubBuf.pLastBlock=pb;
   s_pubBuf.pEndBlock=s_pubBuf.pFirstBlock;
   s_pubBuf.pLastBuffer=s_pubBuf.pFirstBuffer;

   pb= s_pubBuf.pFirstBlock; 
   char* pc = temp->pcData;
   for (int i = 0; i < temp->iSize; ++ i)
   {
      pb->pcData = pc;
      pb = pb->pNext;
      pc += BLOCK_LENGTH;
   }   
   return 0;
}
Block*  buffer_pub_get()
{
  if(s_pubBuf.pEndBlock->pNext==s_pubBuf.pLastBlock)
  	{
  	   buffer_pub_increase();
  	  
  	}
   s_pubBuf.pEndBlock=s_pubBuf.pEndBlock->pNext;
   return s_pubBuf.pEndBlock;  	   
   
}

int buffer_pub_increase()
{
  Buffer* ppb=(struct Buffer*)malloc(sizeof(Buffer));
   ppb->iSize=BLOCK_NUM;
   ppb->pcData=(char*)malloc(ppb->iSize*BLOCK_LENGTH);
   ppb->pNext=NULL;
 
   Block* pFirstpb=(struct Block*)malloc(sizeof(Block));
   Block*pb=pFirstpb;
   
   for (int i = 1; i <ppb->iSize; ++ i)
   {
      pb->pNext = (struct Block*)malloc(sizeof(Block));
      pb->iLength= 0;
      pb = pb->pNext;
   }
    s_pubBuf.pLastBlock->pNext=pFirstpb;
    s_pubBuf.pLastBlock=pb;
   pb->pNext = s_pubBuf.pFirstBlock;  
   s_pubBuf.pLastBuffer->pNext=ppb;
   s_pubBuf.pLastBuffer=ppb;
   
   pb=pFirstpb;
   char* pc = ppb->pcData;
   for (int i = 0; i < ppb->iSize; ++ i)
   {
      pb->pcData = pc;
      pb = pb->pNext;
      pc += BLOCK_LENGTH;
   }   
 return 0;
}
int buffer_pub_uninit()
{
    Block* pFirst= s_pubBuf.pFirstBlock;
    Block* pb = pFirst->pNext;
   while (pb != pFirst)
   {
      Block* temp = pb;
      pb = pb->pNext;
      free(temp);      
   }
   free(pFirst); 


   Buffer* ppb=s_pubBuf.pFirstBuffer; 
   while (ppb!= NULL)
   {
      Buffer* temp = ppb;
      ppb = ppb->pNext;
      free(temp->pcData);
      free(temp);
   }
return 0;
}


/*int buffer_relay_all_client()
{
    if(s_pubBuf.pFirstBlock==s_pubBuf.pEndBlock)
		return 0;
    Block* temp=s_pubBuf.pFirstBlock;
	
    int i=0;
    while(i<ONCE_SEND)
    	{
    	   i++;	        
	  socket_manager_relay_all_client(temp->pcData,temp->iLength);
	  if(temp==s_pubBuf.pEndBlock)
	  	break;
	  s_pubBuf.pLastBlock=temp;
	  temp=temp->pNext;
    	}
	s_pubBuf.pFirstBlock=temp;	
	return 0;
}*/

char* buffer_get_pub_char()
{
   return s_pubChar;
}

