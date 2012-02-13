#include "comRcv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "cJSON.h"

void sensorAction(char* mac_address,char * action)
{
printf("mac address : %s , Action : %s \n",mac_address,action);
}
void getValue(char * mac_address)
{
printf("mac address : %s , Value : 42 \n",mac_address);
}
void activateRT(int interval)
{
printf("Activating Real Time mode for : %d seconds \n",interval);
}

int communicationParse(char* trame)
{

	cJSON *data = cJSON_Parse(trame);
	char* msg_type=NULL;
	char* mac_address=NULL;
	char* action=NULL;
	int interval=0;

	msg_type=cJSON_GetObjectItem(data,"msg_type")->valuestring;

	if ( strcmp(msg_type,"action") ==0 )
	{	
	mac_address=cJSON_GetObjectItem(data,"mac_address")->valuestring;
	action=cJSON_GetObjectItem(data,"action")->valuestring;
	sensorAction(mac_address,action);
	}
	else if ( strcmp(msg_type,"last_state") ==0)
	{
	mac_address=cJSON_GetObjectItem(data,"mac_address")->valuestring;
	getValue(mac_address);
	}
	else if ( strcmp(msg_type,"realtime") ==0)
	{
	interval=cJSON_GetObjectItem(data,"interval")->valueint;
	activateRT(interval);
	}
	else
	{
	printf("Commande inconnue\n");
	}

	cJSON_Delete(data);
	free(msg_type);
	
	return 0;
}



int main ()
{

	cJSON *data = cJSON_CreateObject();
	char * msg = NULL;
	char choosen_tram[10];
	char* mac_address="42424242424242";
	char* action="Do something";
	int interval=42;

	printf("What do you want to send ?\n type action for an action \nvalue for a value and \nrealtime for real time mode \n");
	scanf("%s",choosen_tram);

	if ( strcmp(choosen_tram,"action") == 0 )
	{
	cJSON_AddStringToObject(data,"msg_type","action");
	cJSON_AddStringToObject(data,"mac_address",mac_address);
	cJSON_AddStringToObject(data,"action",action);
	}
	else if ( strcmp(choosen_tram,"value") == 0 )
	{
	cJSON_AddStringToObject(data,"msg_type","last_state");
	cJSON_AddStringToObject(data,"mac_address",mac_address);
	}
	else if ( strcmp(choosen_tram,"realtime") == 0 )
	{
	cJSON_AddStringToObject(data,"msg_type","realtime");
	cJSON_AddNumberToObject(data,"interval",interval);
	}
	else
	{
	printf("Unknown Commande\n");
	}
	
	msg = cJSON_Print(data);
	printf("Parsing\n");
	communicationParse(msg);
	
	/* Clean data */
	free(msg);

	cJSON_Delete(data);
	
	return(0);
}
