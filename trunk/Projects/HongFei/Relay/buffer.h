#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _BUFFER_
#define _BUFFER_

#include <sys/types.h>

#define BLOCK_LENGTH 1024
#define BLOCK_NUM 100
#define ONCE_SEND 5
typedef struct _DataBuf
{   
   char *m_buf;
   size_t m_pos;
   size_t m_size;
   size_t m_maxsize;
}DataBuf;

int buffer_data_init(DataBuf* _dataBuf);
int buffer_data_copy(DataBuf* _dataBuf,const char *buf,size_t buflen);
int buffer_data_uninit(DataBuf* _dataBuf);

 typedef struct _Block
   {
      char* pcData;                  
      int iLength;                         
      struct _Block* pNext;                 
   }Block;

typedef struct _Buffer
   {
      char* pcData;			
      int iSize;			
      struct _Buffer* pNext;
   } Buffer;
typedef struct _PublicBuf
{  
   
   Block *pFirstBlock,*pEndBlock,*pLastBlock;      
   Buffer *pFirstBuffer,*pLastBuffer;		
   int m_iSize;			
   int m_iCount;			
}PublicBuf;
/*
 ___________________________       ___________________________ 
|                  Buffer1                 |->|                   Buffer2               |
 _______      _______       _______    _______      _______      _______
| Block1 |->| Block2 |->| Block3 |  | Block4 |->| Block5|->| Block6 |
*/
int buffer_pub_init();
Block*  buffer_pub_get();
int buffer_pub_increase();
int buffer_pub_uninit();
char* buffer_get_pub_char();

//int buffer_relay_all_client();
#endif
#ifdef  __cplusplus
}
#endif