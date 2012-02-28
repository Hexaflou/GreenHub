#include "gCommunication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comIncludes.h"
#include "comSendTask.h"
#include "comRcvTask.h"
#include <time.h>

#include "../lib/cJSON.h"

/************************  PRIVATE DECLARATION  ***********************/
static SOCKET sock;
static struct hostent *hostinfo;
static SOCKADDR_IN sin = {0}; /* initialise la structure avec des 0 */
static mqd_t smq;
static int userId;

/**************************  PUBLIC FUNCTIONS  ************************/
int gCommunicationInit(int auserId) {
    cJSON *init = cJSON_CreateObject();
    char * msg = NULL;
    
    userId = auserId;
    
    /* Creation du socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket()");
        return INVALID_SOCKET;
    }

    /* Connexion au serveur */
    hostinfo = NULL;

    hostinfo = gethostbyname(SRV_IP); /* on récupère les informations de l'hôte auquel on veut se connecter */
    if (hostinfo == NULL) /* l'hôte n'existe pas */ {
        fprintf(stderr, "Unknown host \n");
        fprintf(stderr,"[gCommunication] Erreur lors de la tentative de connexion au serveur (Hôte inconnu).\n");
        return SOCKET_ERROR;
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
    sin.sin_port = htons(SRV_PORT); /* on utilise htons pour le port */
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
    free(msg);
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
    free(msg);
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
    free(msg);
}


