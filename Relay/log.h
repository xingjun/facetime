#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _LOG_H_
#define _LOG_H_
#include <stdarg.h>
#include <stdio.h>
#define _RELAY_LOG_DEBUG 0
#define _RELAY_LOG_MSG   1
#define _RELAY_LOG_WARN  2
#define _RELAY_LOG_ERROR   3
#define _MAX_PRIORITY 3
#define _MAX_LOG_BUFLEN 8192


#ifdef RL_HAS_MSG
#define RL_MSG(...)          relay_log( _RELAY_LOG_MSG,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#else
#define RL_MSG(...)          (void(0))
#endif

#ifdef RL_HAS_DEBUG
#define RL_DEBUG(...)         relay_log( _RELAY_LOG_DEBUG,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#else
#define RL_DEBUG(...)          (void(0))
#endif

#ifdef RL_HAS_WARN
#define RL_WARN(...)           relay_log( _RELAY_LOG_WARN,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#define RL_WARNX(...)           relay_logx( _RELAY_LOG_WARN,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#else
#define RL_WARN(...)          (void(0))
#define RL_WARNX(...)          (void(0))
#endif

#ifdef RL_HAS_ERROR
#define RL_ERROR(...)           relay_log( _RELAY_LOG_ERROR,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#define RL_ERRORX(...)           relay_logx( _RELAY_LOG_ERROR,__FILE__, __LINE__, __PRETTY_FUNCTION__,__VA_ARGS__) 
#else
#define RL_ERROR(...)          (void(0))
#define RL_ERRORX(...)          (void(0))
#endif

void relay_log(int _priority,const char * szFile, int iLine, const char * szFunction,const char *fmt, ...);
void relay_logx(int _priority,const char * szFile, int iLine, const char * szFunction,const char *fmt, ...);

void set_logname(char* name);
void refresh_log();

int relay_snprintf(char *buf, size_t buflen, const char *format, ...);
int relay_vsnprintf(char *buf, size_t buflen, const char *format, va_list ap);
#endif
#ifdef  __cplusplus
}
#endif
