#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "comIncludes.h"
#include "comRcvTask.h"
#include "../lib/cJSON.h"
#include "../realtime.h"
#include <errno.h>
#include <semaphore.h>
#include "gCommunication.h"
#include "../sensors/ComponentInterface.h"
#include "../sensors/Sensor.h"
#include "../sensors/Actuator.h"
#include <../../libs/gMemory/gMemory.h>

/****************************PRIVATE DECLARATION***********************/
static void * comRcvTask(void * attr);

/* fonction de parsage des donnees */
int communicationParse(char* trame);

/* fonctions de traitement particulieres */
static void getSensorValue(char * hardware_id);
static void getActuatorValue(char * hardware_id);
static void activateRT(int interval);
static void addComponent(cJSON * data);

/* Boucle infinie tant que la connection n'est pas établie 
 * définie dans gCommunication */
void gCommunicationReco();

static SOCKET sock;
static pthread_t comRcvT;
static int gCommunicationStateVA = 1;

/*********************************PUBLIC FUNCTIONS*********************/

int gCommunicationStateGet()
{
	return gCommunicationStateVA;
}

/* Initialisation de la tache */
int comRcvTaskInit(int socket) {
    sock = (SOCKET) socket;
    printf("[gCommunication] Initialisation de la tâche de réception.\n");
    return pthread_create(&comRcvT, NULL, comRcvTask, (void *) NULL);
}

/* Destruction de la tache */
int comRcvTaskClose() {
    return pthread_cancel(comRcvT);
}

void comRcvUpdateSocket(int socket)
{
	sock = (SOCKET) socket;
}

/******************************* PRIVATE ******************************/
static void * comRcvTask(void * attr) {
    char data[2048];
    char temp[2048];
    char * pch = NULL;
    int index = 0;
    int size = 0;
    int length = 0;

    while (1) {
        /*Recuperation d'un datagramme a la suite de l'ancien*/
        size = recv(sock, data + index, 2040 - index - 1, 0);
        if (size <= 0) {
            perror("[gCommunication] Erreur de socket ");
			if(size == 0 || errno==ECONNREFUSED || errno == ENOTCONN)
			{
				/* La connection a été perdu */
				gCommunicationStateVA = 0;
				closesocket(sock);
				gCommunicationReco();
				gCommunicationStateVA = 1;
			}
			else {
				sleep(2);
			}
        }
        else {
            /*Separation de ce qui reste*/
            pch = strtok(data, "}");
            index = 0;
            while (pch != NULL) {
                length = strlen(pch);
                /* si on fini ce tour l'index est a la fin du
                 * json courant                             */
                index += length;
                /* On copie le json dans temp pour y rajouter un }*/
                strncpy(temp, pch, length);
                temp[length] = '}';
                temp[length + 1] = '\0';
                /* On traite le json */
                communicationParse(temp);
                /* On lit la suite */
                pch = strtok(NULL, "}");
            }

            /* On conserve les donnees non traitee */
            if (size > index) {
                strncpy(temp, data + index, 2040 - index);
                strncpy(data, temp, 2040);
                index = strlen(data);
            } else
                index = 0;
        }
    }
    return (void *) NULL;
}

int communicationParse(char* trame) {
    cJSON *data = cJSON_Parse(trame);
    char* msg_type = NULL;
    char* hardware_id = NULL;
    char * comp_type = NULL;
    double value;
    int interval = 0;

    if (data == NULL) {
        fprintf(stderr, "[gCommunication] Unvalid json received.\n");
        return -1;
    }

    msg_type = cJSON_GetObjectItem(data, "msg_type")->valuestring;
	/* On cherche l'action a effectuer selon le type du message */
	
	/* Message d'action */
    if (strncmp(msg_type, "action", 7) == 0) {
        hardware_id = cJSON_GetObjectItem(data, "hardware_id")->valuestring;
        value = cJSON_GetObjectItem(data, "action")->valuedouble;
        ActionActuator(hardware_id, value);
        
    /* Message de demande d'état */
    } else if (strncmp(msg_type, "last_state", 11) == 0) {
        comp_type = cJSON_GetObjectItem(data, "comp_type")->valuestring;
        hardware_id = cJSON_GetObjectItem(data, "hardware_id")->valuestring;
        /* L'opération est différente selon que l'on ait un capteur ou 
         * que l'on ait un actionneur*/
        if (strncmp(comp_type, "sensor", 7) == 0)
            getSensorValue(hardware_id);
        else
            getActuatorValue(hardware_id);
            
    /* Message d'ajout de composant */
    } else if (strncmp(msg_type, "add_comp", 9) == 0) {
        addComponent(data);
    
    /* Action sur le mode realtime*/
    } else if (strncmp(msg_type, "realtime", 9) == 0) {
        interval = cJSON_GetObjectItem(data, "interval")->valueint;
        activateRT(interval);
        
    } else {
        fprintf(stderr, "[gCommunication] Commande inconnue reçue du serveur.\n");
    }

    cJSON_Delete(data);

    return 0;
}

void getSensorValue(char * hardware_id) {

    Sensor* tempSensor = NULL;
    sem_t semSensorList;

    tempSensor = getSensorList();
    semSensorList = getSemSensor();

    sem_wait(&semSensorList);

    while (tempSensor != NULL) {
        if (strncmp(tempSensor->id, hardware_id, 10) == 0) {
            gCommunicationSendValue(tempSensor->id, tempSensor->value);
            /* On a retrouvé le bon sémaphore on peut s'arrêter */
            sem_post(&semSensorList);
            return;
        } else {
            tempSensor = tempSensor->next;
        }
    }

    sem_post(&semSensorList);

}

void getActuatorValue(char * hardware_id) {
    float value;
    GetStatusFromActuator(hardware_id, &value);
    gCommunicationSendValue(hardware_id, value);
}

void activateRT(int interval) {
    printf("[gBox] Activation du mode temps reel pour : %d seconds \n", interval);
    gRTSetPeriod(interval);
}

void addComponent(cJSON * data)
{
	char * hardware_id = NULL;
	int org = 0;
	int funct = 0;
	int type = 0;
	int result = 0;
	
	/* pour le retour */
	cJSON *tosend = cJSON_CreateObject();
	char * msg = NULL;
	
	/* Recupération des données reçus */
	hardware_id = cJSON_GetObjectItem(data, "hardware_id")->valuestring;
	org = cJSON_GetObjectItem(data, "org")->valueint;
	funct = cJSON_GetObjectItem(data, "funct")->valueint;
	type = cJSON_GetObjectItem(data, "type")->valueint;
	
	/* On essaye d'ajouter le capteur */
	result = AddComponent(hardware_id, org, funct, type);
	
	/* préparation de la réponse */
	cJSON_AddStringToObject(tosend, "msg_type", "add_comp");
	
	switch(result)
	{
		case 0:
			cJSON_AddStringToObject(tosend, "result", "ok");
		break;
		
		case -1:
			cJSON_AddStringToObject(tosend, "result", "unsupported");
		break;
		
		case -2:
			cJSON_AddStringToObject(tosend, "result", "notfound");
		break;
		
		case -3:
			cJSON_AddStringToObject(tosend, "result", "existing");
		break;
	}
	
	gCommunicationSend(msg);
	
	/* Clean data */
    gfree(msg);
    cJSON_Delete(tosend);
}
