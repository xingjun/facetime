#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _RL_STREAM_H_
#define _RL_STREAM_H_
#include <stdbool.h>
#include <sys/types.h>

#define BINARY_PACKLEN_LEN 2

typedef struct _BinaryStream
{
       char*  ptr;
	size_t len;
	char* cur;
} BinaryStream;

void  stream_init(BinaryStream* _stream,char* buf,size_t buflen);
bool stream_read(BinaryStream* _stream,char* buf,size_t buflen,size_t *len);
bool stream_read_int(BinaryStream* _stream,int* i);
bool stream_read_short(BinaryStream* _stream,short* i);
bool  stream_read_char(BinaryStream* _stream, char * c);
bool stream_readLengthWithoutOffset(BinaryStream* _stream,size_t * outlen);

bool stream_write(BinaryStream* _stream,char* buf,size_t len);
bool  stream_write_int(BinaryStream* _stream,int  i);
bool  stream_write_short(BinaryStream* _stream,short  i);
bool stream_write_char(BinaryStream* _stream, char  c);
bool stream_write_length(BinaryStream* _stream,size_t length);
void stream_write_flush(BinaryStream* _stream);

#endif /* _MIN_HEAP_H_ */
#ifdef  __cplusplus
}
#endif

