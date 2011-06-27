#include <netinet/in.h>
#include <string.h>
#include "stream.h"
void stream_init(BinaryStream* _stream,char* buf,size_t buflen)
{
   _stream->ptr=buf;
   _stream->len=buflen;
   _stream->cur=buf;
   _stream->cur += BINARY_PACKLEN_LEN;
}


bool stream_read(BinaryStream* _stream,char* buf,size_t buflen,size_t *len)
{
       size_t fieldlen;
	if ( !stream_readLengthWithoutOffset(_stream,&fieldlen) ) {
		return false;
	}
	// user buffer is not enough
	if ( buflen < fieldlen ) {
		return false;
	}
	// 偏移到数据的位置
	_stream->cur += BINARY_PACKLEN_LEN;		
       memcpy(buf, _stream->cur, fieldlen);
	*len = fieldlen;
	_stream->cur += *len;
	return true;
}
bool stream_read_int(BinaryStream* _stream,int* i)
{
     const int VALUE_SIZE = sizeof(int);

	if ( _stream->cur + VALUE_SIZE > _stream->ptr + _stream->len ) {
		return false;
	}

	memcpy(i, _stream->cur, VALUE_SIZE);
	*i = ntohl(*i);

	_stream->cur += VALUE_SIZE;
	
	return true;
}
	
bool stream_read_short(BinaryStream* _stream,short * i)
	{
     const int VALUE_SIZE = sizeof(short);

	if ( _stream->cur + VALUE_SIZE > _stream->ptr + _stream->len ) {
		return false;
	}

	memcpy(i, _stream->cur, VALUE_SIZE);
	*i = ntohs(*i);

	_stream->cur += VALUE_SIZE;
	
	return true;
}
bool stream_read_char(BinaryStream* _stream, char * c)
	{
	const int VALUE_SIZE = sizeof(char);

	if ( _stream->cur + VALUE_SIZE > _stream->ptr + _stream->len ) {
		return false;
	}

	memcpy(c, _stream->cur, VALUE_SIZE);
	_stream->cur += VALUE_SIZE;
	
	return true;
}

bool stream_readLengthWithoutOffset(BinaryStream* _stream,size_t * outlen)
{
   if ( _stream->cur + BINARY_PACKLEN_LEN > _stream->ptr + _stream->len ) {
		return false;
	}

	unsigned short tmp;
	memcpy(&tmp, _stream->cur, sizeof(tmp));
	*outlen = ntohs(tmp);
	return true;
}
bool stream_write(BinaryStream* _stream,char* buf,size_t len)
{
 if ( _stream->cur + len + BINARY_PACKLEN_LEN > _stream->ptr + _stream->len ) {
		return false;
	}

	if ( !stream_write_length(_stream,len) ) {
		_stream->cur -= BINARY_PACKLEN_LEN;
		return false;
	}

	memcpy(_stream->cur, buf, len);
	_stream->cur += len;

	return true;
}
bool stream_write_int(BinaryStream* _stream,int  i)
{
if (_stream->cur + sizeof(i) > _stream->ptr + _stream->len) {
		return false;
	}

	int iTmp = htonl(i);
	memcpy(_stream->cur, &iTmp, sizeof(iTmp));
	_stream->cur += sizeof(iTmp);

	return true;
}
bool  stream_write_short(BinaryStream* _stream,short  i)
{
if (_stream->cur + sizeof(i) > _stream->ptr + _stream->len) {
		return false;
	}

	short iTmp = htons(i);
	memcpy(_stream->cur, &iTmp, sizeof(iTmp));
	_stream->cur += sizeof(iTmp);

	return true;
}
bool stream_write_char(BinaryStream* _stream, char  c)
{
if (_stream->cur + sizeof(c) > _stream->ptr + _stream->len) {
		return false;
	}

	*(_stream->cur) = c;
	++(_stream->cur);
	return true;
}
bool stream_write_length(BinaryStream* _stream,size_t length)
{
        unsigned short ulen = length;

	ulen = htons(ulen);
	memcpy(_stream->cur, &ulen, sizeof(ulen));
	_stream->cur += sizeof(ulen);
	return true;
}
void stream_write_flush(BinaryStream* _stream)
{
	unsigned short datalen =  _stream->cur - _stream->ptr;
	datalen = htons(datalen);
	unsigned short* ps = (unsigned short*)(_stream->ptr);
	*ps = datalen;
}



