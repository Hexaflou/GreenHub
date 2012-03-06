#include "gCommunication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comIncludes.h"
#include "comSendTask.h"
#include "comRcvTask.h"
#include <time.h>

#include "../lib/cJSON.h"
#include <../../libs/gMemory/gMemory.h>

/************************  PRIVATE DECLARATION  ***********************/
static SOCKET sock;
static struct hostent *hostinfo;
static SOCKADDR_IN sin = {0}; /* initialise la structure avec des 0 */
static mqd_t smq;
static int userId;
static int serverPortInt;
static char* serverIPTxt;

/**************************  PUBLIC FUNCTIONS  ************************/
int gCommunicationInit(char * configFileName) {
	cJSON * root, *userID, *serverPort, *serverIP;
	cJSON *init = cJSON_CreateObject();	
	char * msg = NULL;
	char c;
	char txt[CONFIG_CJSON_LENGTH];
	FILE * f;
	int nbOpenedAccolade;
	
	/* Ouverture du fichier en lecture */
	f = fopen(configFileName, "r");
	if (f == NULL) {
		printf("[ComponentInterface] Erreur dans l'ouverture du fichier de configuration %s. \n",configFileName);
		return ERROR;
	}
	/* Récuperation des informations de chaque capteur */
	c = fgetc(f);
	while (c != EOF) {
		memset(txt, 0, sizeof (txt));
		/* Lecture jusqu'au debut de l'objet json :*/
		while (c != '{' && c != EOF) {
			c = fgetc(f);
		}
		if (c == EOF)
			break;
		sprintf(txt, "%s%c", txt, c);

		/* Lecture de l'objet JSON : */
		nbOpenedAccolade = 1;
		while (nbOpenedAccolade > 0) {
			c = fgetc(f);
			if (c == EOF)
				break;
			sprintf(txt, "%s%c", txt, c);
			if (c == '{') {
				nbOpenedAccolade++;
			} else if (c == '}') {
				nbOpenedAccolade--;
			}
		}
		if (c == EOF)
			break;
		sprintf(txt, "%s%c", txt, '\0');
		/* Recuperation des donnees du json */
		root = cJSON_Parse(txt);
		if (root != NULL) {			
			/* Server IP */
			serverIP = cJSON_GetObjectItem(root, "serverIP");
			if (serverIP == NULL) {
				printf("[gCommunication] IP du serveur invalide dans le fichier %s\n", configFileName);
				return ERROR;
			}
			serverIPTxt = (char*)gmalloc(sizeof(char)*strlen(serverIP->valuestring)+1);
			strcpy(serverIPTxt,serverIP->valuestring);

			/* Server Port */
			serverPort = cJSON_GetObjectItem(root, "serverPort");
			if (serverPort == NULL) {
				printf("[gCommunication] Port du serveur invalide dans le fichier %s\n", configFileName);
				return ERROR;
			}
			serverPortInt = serverPort->valueint;			

			/* User ID */
			userID = cJSON_GetObjectItem(root, "userID");
			if (userID == NULL) {
				printf("[gCommunication] ID de l'utilisateur invalide dans le fichier %s\n", configFileName);
				return ERROR;
			}
			userId = userID->valueint;			
		}else{
			perror("[gCommunication] Erreur dans le parsage du fichier de configuration");
			return ERROR;
		}
	}


		/* Creation du socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			perror("socket()");
			return INVALID_SOCKET;
		}

		/* Connexion au serveur */
		hostinfo = NULL;

		hostinfo = gethostbyname(serverIPTxt); /* on récupère les informations de l'hôte auquel on veut se connecter */
		if (hostinfo == NULL) /* l'hôte n'existe pas */ {
			fprintf(stderr, "Unknown host \n");
			fprintf(stderr,"[gCommunication] Erreur lors de la tentative de connexion au serveur (Hôte inconnu).\n");
			return SOCKET_ERROR;
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(serverPortInt); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;

	if (connect(sock, (SOCKADDR *) & sin, sizeof (SOCKADDR)) == SOCKET_ERROR) {
		printf(stderr,"[gCommunication] Erreur lors de la tentative de connexion au serveur.\n");
		return SOCKET_ERROR;
	}

	/* Creation du service de reception puis d'emmission */
	comRcvTaskInit(sock);
	smq = comSendTaskInit(sock);

	/* Envoi des informations de login */
	cJSON_AddStringToObject(init, "msg_type", "login");
	cJSON_AddNumberToObject(init, "user_id", userId);
	msg = cJSON_Print(init);

	gCommunicationSend(msg);

	cJSON_Delete(init);
	gfree(msg);
	return 0;
}

int gCommunicationSend(char * msg) {
	return mq_send(smq, msg, MAX_MQ_SIZE, 0);
}

int gCommunicationSendValue(char hwid[12], double value) {
	cJSON *data = cJSON_CreateObject();
	char * msg = NULL;
	int ret;

	cJSON_AddStringToObject(data, "msg_type", "new_state");
	cJSON_AddStringToObject(data, "hardware_id", hwid);
	cJSON_AddNumberToObject(data, "new_value", value);
	cJSON_AddNumberToObject(data, "date", (int) time(NULL));


	msg = cJSON_Print(data);

	ret = gCommunicationSend(msg);

	/* Clean data */
	gfree(msg);
	cJSON_Delete(data);
	return ret;
}

int gCommunicationClose() {
	comSendTaskClose();
	comRcvTaskClose();
	closesocket(sock);
	return 0;
}


int gCommunicationIsAlive()
{
	return gCommunicationStateGet();
}

void gCommunicationReco()
{
	int i = 0;
	cJSON *init = cJSON_CreateObject();
	char * msg = NULL;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	comSendUpdateSocket(sock);
	comRcvUpdateSocket(sock);
	while (connect(sock, (SOCKADDR *) & sin, sizeof (SOCKADDR)) == SOCKET_ERROR) {
		i++;
		fprintf(stderr,"[gCommunication] Tentative de connection au serveur n°%d échoué.\n",i);
		sleep(10);
	}

	/* renvoi des informations de login */
	cJSON_AddStringToObject(init, "msg_type", "login");
	cJSON_AddNumberToObject(init, "user_id", userId);
	msg = cJSON_Print(init);

	if (send(sock, msg, strlen(msg), 0) < 0) {
		perror("[gCommunication] Impossible d'envoyer un message de login. (WTF)");
	}

	cJSON_Delete(init);
	gfree(msg);
}


