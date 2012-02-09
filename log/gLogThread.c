#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "gCommunication.h"
#include "gLogThread.h"

typedef struct {
	char mac[40];
	double value;
	int date;
} GInformation;

/***************************PRIVATE DECLARATION***********************/
static void * gLogFunc(void *);
/* variables */
static int run;
pthread_t gLogThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* rLogFile = NULL;
static FILE* wLogFile = NULL;
static FILE* stateFile = NULL;
static int position = 0;

/************************PUBLIC***************************************/
int gLogsLog (char mac[40], double value)
{
	GInformation info;/* = {mac, value, 0};*/
	strcpy(info.mac,mac);
	info.value = value;
	info.date = (int)time(NULL);
	if(wLogFile !=NULL)
	{
		pthread_mutex_unlock( &mutex );
		fwrite(&info,sizeof(GInformation),1,wLogFile);
		pthread_mutex_lock( &mutex );
	}
	else
	{
		printf("Error : impossible to log, files are not initialized");
		return -1;
	}
	return 0;
}

int gLogThreadInit()
{
	run = 1;
	wLogFile = fopen(LOG_FILENAME,"ab");
	rLogFile = fopen(LOG_FILENAME,"rb");
	/*Si l'etat de lecture existe on le recupere */
	stateFile = fopen(LOG_STATE_FILENAME,"rb");
	if(stateFile!=NULL)
	{
		fread(&position,sizeof(int),1,stateFile);
		fclose(stateFile);
	}
	stateFile = fopen(LOG_STATE_FILENAME,"wb");
	fseek(rLogFile,position,SEEK_SET);
	return pthread_create( &gLogThread, NULL, gLogFunc, (void *) NULL);
}

int gLogThreadClose()
{
	run = 0;
	return 0;
}

/************************PRIVATE***************************************/
static int send(GInformation * info)
{
	cJSON *data = cJSON_CreateObject();
	char * msg = NULL;
	int ret;
	
	cJSON_AddStringToObject(data,"msg_type","new_state");
	cJSON_AddStringToObject(data,"mac_address",info->mac);
	cJSON_AddNumberToObject(data,"new_value",info->value);
	cJSON_AddNumberToObject(data,"date",info->date);
	
	
	msg = cJSON_Print(data);
	
	ret = gCommunicationSend(msg);
	
	/* Clean data */
	free(msg);
	cJSON_Delete(data);
	return ret;
}
static void * gLogFunc(void * attr)
{
	GInformation info;
	while(run)
	{
		pthread_mutex_lock( &mutex );
		while (!feof(rLogFile))
		{
			fread(&info,sizeof(GInformation),1,rLogFile);
			send(&info);
		}
		
		position = ftell (rLogFile);
		fwrite(&position,sizeof(int),1,stateFile);
		pthread_mutex_unlock( &mutex );
		
		/* wait for next time */
		sleep(LOG_SEND_PERIOD);
	}
	
	pthread_mutex_lock( &mutex );
	fclose(rLogFile);
	fclose(wLogFile);
	fclose(stateFile);
	wLogFile=NULL;
	pthread_mutex_unlock( &mutex );
	return 0;
}
	
