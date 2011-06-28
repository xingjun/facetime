#include <stdio.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "config/config.h"
#include "epoll.h"
#include "timer.h"


void prog_exit(int signo);
void system_stop();
void set_limit();
int  server_listen();
void   server_connect();
RLConfig g_rlConfig;
static  FDHandler s_listenSocket[2];
static FDHandler  s_stateSocket[STATE_NUM];
int main()
{
       set_limit();
	
	int do_dameon=0;
	if(do_dameon)
	{
		int pid;
		signal(SIGCHLD, SIG_IGN);
		pid = fork();
		if(pid < 0) {
			perror("fork");
			exit(-1);
		}
		else if(pid > 0)
			exit(0);
		setsid();
	}
	signal(SIGCHLD, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, prog_exit);
	signal(SIGKILL, prog_exit);
	signal(SIGTERM, prog_exit);

      
       char _configfilename[128]={0};	
	strcpy(_configfilename,"../etc/relay.xml");	
	printf("configfilename = %s\n",_configfilename);
	ReadConfigFromFile(_configfilename,&g_rlConfig);
	char logname[128]={0};//"/home/jinyiming/gameapp/log/relay";
	strcpy(logname,g_rlConfig.logDir);
	strcat(logname,"relay");
	set_logname(logname);    
       timer_init();
	if(epoll_init()!=0)
		{
		   RL_ERRORX("epoll_init error");
		   return -1;
		}
	server_connect();
	if(server_listen()!=0)
		{ 
		   RL_ERRORX("Listen error");
		   return -1;
		}	
	//buffer_pub_init();//暂不用
	//buffer_pub_increase();//test
	epoll_run(); 
	//sleep(5);//test

	system_stop();
	return 0;
}

void prog_exit(int signo)
{
      printf("receive sign = %d\n",signo);
       
	signal(SIGINT, SIG_IGN);
	signal(SIGKILL, SIG_IGN);
	signal(SIGTERM, SIG_IGN);	
	system_stop();
}
void system_stop()
{
     refresh_log();
     epoll_stop();
     //buffer_pub_uninit();//暂不用
     //timer_stop();
}
void set_limit()
{
    struct rlimit limit;
	char p = '1';
	limit.rlim_cur = RLIM_INFINITY;
	limit.rlim_max = RLIM_INFINITY;
	if(setrlimit(RLIMIT_CORE, &limit))
	{
		printf("set limit failed\n");
	}
	printf("p = %d\n",p);
}
int  server_listen()
{
		s_listenSocket[0].nFDType=FD_TYPE_LISTEN_SERVER;
		if(socket_listen(&s_listenSocket[0],g_rlConfig.hostS,g_rlConfig.listenS)!=0)
		{RL_ERRORX("Server Listen error");return -1;}		
		s_listenSocket[1].nFDType=FD_TYPE_LISTEN_CLIENT;
		if(socket_listen(&s_listenSocket[1],g_rlConfig.hostC,g_rlConfig.listenC)!=0)
             {RL_ERRORX("Client Listen error");return -1;}
		return 0;  
		}
void   server_connect()
{
   int i=0;
   for(;i<STATE_NUM;i++)
   	{   	 
	  s_stateSocket[i].nFDType=FD_TYPE_CLIENT_STATE;
   	  socket_connect(&s_stateSocket[i],g_rlConfig.sCfg[i].stateip,g_rlConfig.sCfg[i].stateport);   	
   	}
}

