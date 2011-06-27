#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _CRYPT_H_
#define _CRYPT_H_
/********************************************************************

purpose:	
*********************************************************************/
#include <stdbool.h>


#if !defined BYTE
#define BYTE unsigned char
#endif
#if !defined UINT32
#define UINT32 unsigned int
#endif
#if !defined UINT
#define UINT unsigned int
#endif

#if !defined TRUE
#define TRUE true
#endif

#if !defined FALSE
#define FALSE false
#endif

#define TEAFLAG 'T'
#define XTEAFLAG 'X'
#define COMPRESSFLAG 'C'

	
    int XTEAEncrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
    int XTEADecrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
	

    static BYTE DEF_KEY[16] = {'A','3','4','F','9','7','7','1','E','C','1','0','B','C','A','5'};

    /// TEA加密
    /*
    @param	pInBuffer IN			要加密的buffer
    @param	nInBufferLen IN			要加密的buffer的长度
    @param	pOutBuffer IN			存放加密结果的buffer
    @param	pnOutBufferLen IN OUT	存放加密结果的buffer的长度(IN)，返回加密结果的实际大小(OUT)
    @param	pKey IN					加密的key,16byte
    @param	uRounds IN				TEA加密轮数，轮数越大安全性越高，但加密速度越慢
    @remark	加密结果会比加密前的长度大1-8个byte,所以建议 len(存放加密结果的buffer) = len(要加密的buffer) + 8
    */
    
    bool TEAEncrypt_(BYTE* pInBuffer, int nInBufferLen, BYTE* pOutBuffer, int* pnOutBufferLen, BYTE pKey[16], UINT uRounds);
    #define TEAEncrypt(pInBuffer, nInBufferLen, pOutBuffer, pnOutBufferLen,pKey)  \
	     TEAEncrypt_(pInBuffer, nInBufferLen, pOutBuffer, pnOutBufferLen,pKey,16)
    /// TEA解密
    /*
    @param	pInBuffer		IN 要解密的buffer
    @param	nInBufferLen	IN 要解密的buffer的长度
    @param	pOutBuffer		IN 存放解密结果的buffer
    @param	pnOutBufferLen	IN OUT 存放解密结果的buffer的长度(IN)，返回解密结果的实际大小(OUT)
    @param	pKey			IN 解密的key,16byte,必须和加密时的相同
    @param	uRounds			IN 轮数,必须和加密时的相同
    @remark	存放解密结果的buffer的长度 必须要 >= (要解密的buffer的长度 - 1)
    */
     
    bool TEADecrypt_(BYTE* pInBuffer, int nInBufferLen, BYTE* pOutBuffer, int* pnOutBufferLen, BYTE pKey[16], UINT uRounds );
     #define TEADecrypt(pInBuffer, nInBufferLen, pOutBuffer, pnOutBufferLen,pKey)  \
	     TEADecrypt_(pInBuffer, nInBufferLen, pOutBuffer, pnOutBufferLen,pKey,16)
	     
	int  TEAEncryptLen(int nInBufferLen);
      int TEADecryptLen(int nInBufferLen);

	bool StreamDecrypt_(const char *inbuf,int inbuflen,char *outbuf,int *outbuflen,char key[16],int type );
       #define StreamDecrypt(inbuf,inbuflen,outbuf,outbuflen,key) \
		StreamDecrypt_(inbuf,inbuflen,outbuf,outbuflen,key,1)
		
	bool StreamEncrypt_(const char *inbuf,int inbuflen,char *outbuf,int *outbuflen,char key[16],int type );
      #define StreamEncrypt(inbuf,inbuflen,outbuf,outbuflen,key) \
		StreamEncrypt_(inbuf,inbuflen,outbuf,outbuflen,key,1)
		
	bool StreamCompress(const char *inbuf,int inbuflen,char *outbuf,int *outbuflen);

	bool StreamUnCompress(const char *inbuf,int inbuflen,char *outbuf,int *outbuflen);

#endif
#ifdef  __cplusplus
}
#endif

