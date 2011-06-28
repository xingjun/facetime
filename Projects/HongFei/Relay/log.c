#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "log.h"

struct LogInfo
{
 char szLogBuf[_MAX_LOG_BUFLEN];
 int nCurBufLen;
 char szLogShortName[50];
 char szLogFullName[128];
 int nTimeLeft;
 struct timeval stLasttime;
};

static struct LogInfo s_logInfo;


static void _warn_helper(int _priority, int log_errno, const char * szFile, int iLine, const char * szFunction,const char *fmt,
                         va_list ap);
static void relay_writelog();
static void check_logname();
static void change_logname();
static void format_date();
static void format_pri_info(int _priority);
static void format_file_info(const char * szFile, int iLine, const char * szFunction);

void relay_log(int _priority,const char * szFile, int iLine, const char * szFunction,const char *fmt, ...)
{
      va_list ap;	
	va_start(ap, fmt);
	_warn_helper(_priority, -1, szFile, iLine, szFunction,fmt, ap);
	va_end(ap);      
}

void relay_logx(int _priority,const char * szFile, int iLine, const char * szFunction,const char *fmt, ...)
{
      va_list ap;	
	va_start(ap, fmt);
	_warn_helper(_priority, errno, szFile, iLine, szFunction,fmt, ap);
	va_end(ap);      
}

void set_logname(char* name)
{     
     if(strlen(name)>50)
	 	return;
     memcpy(s_logInfo.szLogShortName,name,strlen(name));     
     s_logInfo.nTimeLeft=-1;
}

void refresh_log()
{
    relay_writelog();   
}

int relay_snprintf(char *buf, size_t buflen, const char *format, ...)
{
	int r;
	va_list ap;
	va_start(ap, format);
	r = relay_vsnprintf(buf, buflen, format, ap);
	va_end(ap);
	return r;
}

int relay_vsnprintf(char *buf, size_t buflen, const char *format, va_list ap)
{
	int r = vsnprintf(buf, buflen, format, ap);
	if(buflen<_MAX_LOG_BUFLEN)
		buf[buflen-1] = '\0';
	return r;
}

static void 
_warn_helper(int _priority, int log_errno, const char * szFile, int iLine, const char * szFunction,const char *fmt, va_list ap)
{
	//char buf[1024];
	size_t len=0;

	format_date();
	format_pri_info(_priority);
	if (fmt != NULL)
		len=relay_vsnprintf(s_logInfo.szLogBuf+s_logInfo.nCurBufLen, _MAX_LOG_BUFLEN-s_logInfo.nCurBufLen, fmt, ap);
	else
		s_logInfo.szLogBuf[s_logInfo.nCurBufLen] = '\0';

	s_logInfo.nCurBufLen+=len;
	
	if (log_errno >= 0) {		
		len=relay_snprintf(s_logInfo.szLogBuf+s_logInfo.nCurBufLen, _MAX_LOG_BUFLEN-s_logInfo.nCurBufLen, ": %s",strerror(log_errno));
             s_logInfo.nCurBufLen+=len;	
	}	

	
	
	format_file_info(szFile,iLine,szFunction);
	s_logInfo.szLogBuf[s_logInfo.nCurBufLen++]='\n';

	#ifdef DEBUG
	relay_writelog();
	#endif
	
	if(s_logInfo.nCurBufLen>_MAX_LOG_BUFLEN-300)
		{				
		relay_writelog();
		}
	
}


static void
relay_writelog()
{	
       check_logname();
	int fd = open(s_logInfo.szLogFullName, O_CREAT|O_WRONLY|O_APPEND|O_LARGEFILE, 0644);
	if ( fd < 0) {
		return ;
	}
	write(fd, s_logInfo.szLogBuf, s_logInfo.nCurBufLen);
	close(fd);	
	memset(s_logInfo.szLogBuf, 0,_MAX_LOG_BUFLEN);
	s_logInfo.nCurBufLen=0;
}

static void check_logname()
{
     if(s_logInfo.nTimeLeft==-1)
		{
		   time_t tTime = time(0);		
		   struct tm stTime;
		   localtime_r(&tTime, &stTime);		   
	   	   s_logInfo.nTimeLeft= stTime.tm_hour * 3600 + stTime.tm_min * 60 + stTime.tm_sec;
		   gettimeofday(&s_logInfo.stLasttime,NULL);
		   change_logname();
		}
	 else 
	 	{
	struct timeval nowtime;
	gettimeofday(&nowtime,NULL);
	if((nowtime.tv_sec-s_logInfo.stLasttime.tv_sec)>s_logInfo.nTimeLeft)
		{
		    change_logname();
		    s_logInfo.nTimeLeft=86400;
		    s_logInfo.stLasttime=nowtime;
		}
	 	}
}

static void change_logname()
{
      time_t tTime = time(0);	
	struct tm stTime;
	localtime_r(&tTime, &stTime);
	char szFormatTime[32];
	strftime(szFormatTime, sizeof(szFormatTime)-1, ".%Y-%m-%d", &stTime);
	memcpy(s_logInfo.szLogFullName,"0",sizeof(s_logInfo.szLogFullName));
       relay_snprintf(s_logInfo.szLogFullName,sizeof(s_logInfo.szLogFullName),"%s%s%s",s_logInfo.szLogShortName,szFormatTime,".log");
}
static void format_date()
{
      struct timeval tv;
	gettimeofday(&tv, NULL);

	struct tm stTime;
	localtime_r(&tv.tv_sec, &stTime);
 
	char szFormatTime[128];
	memset(szFormatTime, 0x0, sizeof(szFormatTime));	
	szFormatTime[0]='[';
	strftime(szFormatTime+1, sizeof(szFormatTime)-2, "%Y-%m-%d %H:%M:%S", &stTime);
	size_t len=strlen(szFormatTime);
	szFormatTime[len++]=']';
	memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,szFormatTime,len);
	s_logInfo.nCurBufLen+=len;
}
static void format_pri_info(int _priority)
{
      switch (_priority) {
	case _RELAY_LOG_DEBUG:
			{
		memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,"debug:",6);
		s_logInfo.nCurBufLen+=6;
		}
		break;
	case _RELAY_LOG_MSG:
			{
		memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,"msg:",4);
				s_logInfo.nCurBufLen+=4;
		}
		break;
	case _RELAY_LOG_WARN:
			{
		memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,"warn:",5);	
		s_logInfo.nCurBufLen+=5;
		}
		break;
	case _RELAY_LOG_ERROR:
		{		
		memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,"error:",6);
		s_logInfo.nCurBufLen+=6;
		}
		break;
	default:
			{
		memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,"???:",4);	
		s_logInfo.nCurBufLen+=4;
		}
		break;
	}
}

static void format_file_info(const char * szFile, int iLine, const char * szFunction)
{
      size_t len=strlen(szFile);
      s_logInfo.szLogBuf[s_logInfo.nCurBufLen++]='(';
      memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,szFile,len);
	s_logInfo.nCurBufLen+=len;
	s_logInfo.szLogBuf[s_logInfo.nCurBufLen++]=' ';	
	len=relay_snprintf(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,4,"%d",iLine);
	s_logInfo.nCurBufLen+=len;
      s_logInfo.szLogBuf[s_logInfo.nCurBufLen++]=' ';
      len=strlen(szFunction);
	memcpy(s_logInfo.szLogBuf+s_logInfo.nCurBufLen,szFunction,len);
	s_logInfo.nCurBufLen+=len;
	//printf("s_logInfo.nCurBufLen = %d\n",s_logInfo.nCurBufLen);
	 s_logInfo.szLogBuf[s_logInfo.nCurBufLen++]=')';	
}


