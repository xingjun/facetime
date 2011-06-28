#include "config.h"
#include "tinyxml.h"

int ReadConfigFromFile(char* filename,RLConfig* _config)
{
     TiXmlDocument doc(filename);	
	bool ret = doc.LoadFile();
	if (!ret)
		return -1;

	TiXmlElement *root = doc.FirstChildElement("config");
	if(!root)
		return -1;

	TiXmlElement *servercfg = root->FirstChildElement("servercfg");
	if(!servercfg)
		return -1;

	const char* str = servercfg->Attribute("hostS");
	if(!str)
	{
		printf("read hostS error\n");
		return -1;
	}
	printf("hostS = %s ",str);
	strncpy(_config->hostS,str,sizeof(_config->hostS));	
	
	str = servercfg->Attribute("listenS",(int *)&(_config->listenS));
	if(!str)
	{
		printf("read listenS error\n");
		return -1;
	}
	printf(" listenS = %s ",str);

	str = servercfg->Attribute("hostC");
	if(!str)
	{
		printf("read hostC error\n");
		return -1;
	}
	printf("hostC = %s ",str);
	strncpy(_config->hostC,str,sizeof(_config->hostC));	
	
	str = servercfg->Attribute("listenC",(int *)&(_config->listenC));
	if(!str)
	{
		printf("read listenC error\n");
		return -1;
	}
	printf(" listenC = %s ",str);
	
	str = servercfg->Attribute("maxclient",(int *)&(_config->maxclient));
	if(!str)
	{
		printf("read maxclient error\n");
		return -1;
	}
	printf(" maxclient = %s ",str);
	str = servercfg->Attribute("conntimeout",&(_config->timeout));
	if(!str)
	{
		printf("read conntimeout error\n");
		return -1;
	}
	printf(" timeout = %s ",str);
       str = servercfg->Attribute("timeout2",&(_config->timeout2));
	if(!str)
	{
		printf("read timeout2 error\n");
		return -1;
	}
	printf(" timeout2 = %s ",str);
	

	str = servercfg->Attribute("logdir");
	if(!str)
	{
		printf("read logdir error\n");
		return -1;
	}
	printf(" logdir = %s ",str);
	strncpy(_config->logDir,str,sizeof(_config->logDir));	

	str = servercfg->Attribute("serverkey");
	if(!str)
	{
		printf("read serverkey error\n");
		return -1;
	}
	printf(" serverkey = %s ",str);
	strncpy(_config->serverkey,str,sizeof(_config->serverkey));	

	TiXmlElement *states = root->FirstChildElement("states");
	if(!states)
	{
		printf("read states error\n");
		return -1;
	}

	TiXmlElement *state = states->FirstChildElement("state");
	int i=0;
	while(state)
	{
		str = state->Attribute("ip");
	       if(!str)
	      {
		   printf("read ip error\n");
		   return -1;
	       }
		strncpy(_config->sCfg[i].stateip,str,sizeof(_config->sCfg[i].stateip));
	

	str = state->Attribute("port",(int*)&(_config->sCfg[i].stateport));
	if(!str)
	{
		printf("read port error\n");
		return -1;
	}

	printf("\nstateip = %s , stateport = %d ",_config->sCfg[i].stateip,_config->sCfg[i].stateport);
	state = state->NextSiblingElement("state");
	i++;
	}
	return 0;
}


