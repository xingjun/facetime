#ifdef  __cplusplus
extern "C" {
#endif

#ifndef CONFIG_H_
#define CONFIG_H_
#define STATE_NUM 1

typedef struct _StateCfg
{
      char stateip[20];
	short stateport;
}StateCfg;
typedef struct _RLConfig
{
	char hostS[20];
	short listenS;
	char hostC[20];
	short listenC;
	short maxclient;
	char logDir[128];
	int timeout;
	int timeout2;
	char serverkey[16];
	StateCfg sCfg[STATE_NUM];
}RLConfig;

 
int ReadConfigFromFile(char* filename,RLConfig* _config);
#endif

#ifdef  __cplusplus
}
#endif
