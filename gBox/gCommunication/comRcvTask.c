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

/****************************PRIVATE DECLARATION***********************/
static void * comRcvTask(void * attr);

/* fonction de parsage des donnees */
int communicationParse(char* trame);

/* fonctions de traitement particulieres */
static void getSensorValue(char * mac_address);
static void getActuatorValue(char * mac_address);
static void activateRT(int interval);

static SOCKET sock;
static pthread_t comRcvT;

/*********************************PUBLIC FUNCTIONS*********************/

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
        if (size > 0)
            printf("Données reçus : \n%s\n\n", data);
        else {
            printf("erreur de socket : %s\n", strerror(errno));

            sleep(1);
        }
        if (size > 0) {
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
    char* mac_address = NULL;
    char * comp_type = NULL;
    double value;
    int interval = 0;

    if (data == NULL) {
        fprintf(stderr, "Unvalid json received.\n");
        return -1;
    }

    msg_type = cJSON_GetObjectItem(data, "msg_type")->valuestring;

    if (strncmp(msg_type, "action", 7) == 0) {
        mac_address = cJSON_GetObjectItem(data, "mac_address")->valuestring;
        value = cJSON_GetObjectItem(data, "action")->valuedouble;
        ActionActuator(mac_address, value);
    } else if (strncmp(msg_type, "last_state", 11) == 0) {
        comp_type = cJSON_GetObjectItem(data, "comp_type")->valuestring;
        mac_address = cJSON_GetObjectItem(data, "mac_address")->valuestring;
        if (strncmp(comp_type, "sensor", 6) == 0)
            getSensorValue(mac_address);
        else
            getActuatorValue(mac_address);
    } else if (strncmp(msg_type, "realtime", 9) == 0) {
        interval = cJSON_GetObjectItem(data, "interval")->valueint;
        activateRT(interval);
    } else {
        fprintf(stderr, "Commande inconnue reçue du serveur.\n");
    }

    cJSON_Delete(data);

    return 0;
}

void getSensorValue(char * mac_address) {

    Sensor* tempSensor = NULL;
    sem_t semSensorList;

    tempSensor = getSensorList();
    /* TODO : Penser à passer aux mutex p_thread*/
    semSensorList = getSemSensor();

    sem_wait(&semSensorList);

    while (tempSensor != NULL) {
        if (strncmp(tempSensor->id, mac_address, 10) == 0) {
            gCommunicationSendValue(tempSensor->id, tempSensor->value);
            /* We can return from here (MAC address is unique) */
            sem_post(&semSensorList);
            return;
        } else {
            tempSensor = tempSensor->next;
        }
    }

    sem_post(&semSensorList);

}

void getActuatorValue(char * mac_address) {
    float value;
    GetStatusFromActuator(mac_address, &value);
    gCommunicationSendValue(mac_address, value);
}

void activateRT(int interval) {
    printf("Activating Real Time mode for : %d seconds \n", interval);
    gRTSetPeriod(interval);
}
