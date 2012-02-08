#include "gCommunication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#include <time.h>

#include "cJSON.h"


static SOCKET sock;
static struct hostent *hostinfo;
static SOCKADDR_IN sin  = { 0 };/* initialise la structure avec des 0 */

int gCommunicationInit(int userId)
{
	cJSON *init = cJSON_CreateObject();
	char * msg = NULL;
	/* Creation du socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		return INVALID_SOCKET;
	}

	/* Connexion au serveur */
	hostinfo = NULL;
	
	hostinfo = gethostbyname(SRV_IP); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL) /* l'hôte n'existe pas */
	{
		fprintf (stderr, "Unknown host \n");
		return SOCKET_ERROR;
	}
	
	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(SRV_PORT); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;
	
	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
		return SOCKET_ERROR;
	}
	
	/* Envoi des informations de login */
	cJSON_AddStringToObject(init,"msg_type","login");
	cJSON_AddNumberToObject(init,"user_id",userId);
	msg = cJSON_Print(init);

	gCommunicationSend(msg);
	free(msg);
	cJSON_Delete(init);

	/* TODO:Création des threads utiles */

	return 0;
}

int gCommunicationSend(char * msg)
{
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		perror("send()");
		return SOCKET_ERROR;
	}
	return 0;
}

int gCommunicationClose()
{
	closesocket(sock);
	return 0;
}

int gLogsLog(char mac[40], double value)
{
	cJSON *data = cJSON_CreateObject();
	char * msg = NULL;
	
	cJSON_AddStringToObject(data,"msg_type","new_state");
	cJSON_AddStringToObject(data,"mac_address",mac);
	cJSON_AddNumberToObject(data,"new_value",value);
	cJSON_AddNumberToObject(data,"date",(int)time(NULL));
	
	
	msg = cJSON_Print(data);
	
	gCommunicationSend(msg);
	
	/* Clean data */
	free(msg);
	cJSON_Delete(data);
}

int main ()
{
	int i = 0;
	gCommunicationInit(1);
	for (i = 0 ; i < 50 ; i++)
		gLogsLog("42",i);
	
	gCommunicationClose();
	return 0;
}
