#include "gCommunication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "cJSON.h"

int gCommunicationParse(char* trame)
{
	char* buffer=NULL;
	char* buffer1=NULL;
	double buffer2=0;
	int buffer3=0;
	
	cJSON *data = cJSON_Parse(trame);
	
	buffer=cJSON_GetObjectItem(data,"msg_type")->valuestring;
	buffer1=cJSON_GetObjectItem(data,"mac_address")->valuestring;
	buffer2=cJSON_GetObjectItem(data,"new_value")->valuedouble;
	buffer3=cJSON_GetObjectItem(data,"date")->valueint;
	
	printf("Type : %s\n",buffer);
	printf("mac : %s\n",buffer1);
	printf("value: %d\n",buffer2);
	printf("date : %d\n",buffer3);
	
	free(buffer);
	free(buffer1);
	
	
	cJSON_Delete(data);
	
	return 0;
}


int main ()
{
	double value = 34;
	char mac[40]="bite";
	cJSON *data = cJSON_CreateObject();
	char * msg = NULL;
	
	cJSON_AddStringToObject(data,"msg_type","new_state");
	cJSON_AddStringToObject(data,"mac_address",mac);
	cJSON_AddNumberToObject(data,"new_value",value);
	cJSON_AddNumberToObject(data,"date",(int)time(NULL));
	
	
	msg = cJSON_Print(data);
	
	gCommunicationParse(msg);
	
	/* Clean data */
	free(msg);
	cJSON_Delete(data);
	
	return(0);
}
