#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/cJSON.h"
#include "../libs/gMemory/gMemory.h"
#include "gCommunication/gCommunication.h"
#include "gLogs.h"


/***************************PRIVATE DECLARATION***********************/
static void * gLogFunc(void *);
static int send(char * hwid, double value, int date);
/* variables */
pthread_t gLogThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* rLogFile = NULL;
static FILE* wLogFile = NULL;
static FILE* stateFile = NULL;
static int position = 0;

/************************PUBLIC***************************************/
int gLogsLog(char hwid[12], double value) {
    wLogFile = fopen(LOG_FILENAME, "a");
    if (wLogFile != NULL) {
        pthread_mutex_lock(&mutex);
        fprintf(wLogFile, "HWID: %s ; Value: %f ; Timestamp: %d\n", hwid, value, (int) time(NULL));
        pthread_mutex_unlock(&mutex);
    } else {
        printf("[gLog] Impossible d'initialiser les logs, les fichiers sont inaccessibles.\n");
        return -1;
    }
    fclose(wLogFile);
    return 0;
}

int gLogThreadInit() {
    int i = 0;
    int pos = 0;
    char data[1024];
    position = 0;
    wLogFile = fopen(LOG_FILENAME, "a");
    fclose(wLogFile);
    rLogFile = fopen(LOG_FILENAME, "r"); /* TODO: CHECK IF ITS WORKING (!= NULL) */
    /*Si l'etat de lecture existe on le recupere */
    stateFile = fopen(LOG_STATE_FILENAME, "r");
    if (stateFile != NULL) {
        fscanf(stateFile, "%d", &pos);
        fclose(stateFile);
        for (i = 0; i < pos; i++) {
            if (fgets(data, 1024, rLogFile) != NULL) {
                position++;
            }
        }
    }

    stateFile = fopen(LOG_STATE_FILENAME, "w+");
    fprintf(stateFile, "%d\n", position);
    return pthread_create(&gLogThread, NULL, gLogFunc, (void *) NULL);
}

int gLogThreadClose() {
    pthread_mutex_lock(&mutex);
    pthread_cancel(gLogThread);
    fclose(rLogFile);
    fclose(stateFile);
    wLogFile = NULL;
    pthread_mutex_unlock(&mutex);
    return 0;
}

/************************PRIVATE***************************************/
static int send(char * hwid, double value, int date) {
    cJSON *data = cJSON_CreateObject();
    char * msg = NULL;
    int ret;

    cJSON_AddStringToObject(data, "msg_type", "new_state");
    cJSON_AddStringToObject(data, "hardware_id", hwid);
    cJSON_AddNumberToObject(data, "new_value", value);
    cJSON_AddNumberToObject(data, "date", date);


    msg = cJSON_Print(data);

    ret = gCommunicationSend(msg);

    /* Clean data */
    gfree(msg);
    cJSON_Delete(data);
    return ret;
}

static void * gLogFunc(void * attr) {
    char hwid[40];
    double value = 0.0;
    int date = 0;
    char data[1024];

    while (1) {
		if(gCommunicationIsAlive()) {
			pthread_mutex_lock(&mutex);
			fseek(rLogFile, 0, SEEK_CUR);
			while (!feof(rLogFile)) {
				
				if (fgets(data, 1024, rLogFile) != NULL) {
					sscanf(data, "%*s %s %*c %*s %lf %*c %*s %d", hwid, &value, &date);
					/*printf("Lu : hwid=%s, value=%lf, date=%d (from data=%s)\n", mac, value, date, data);*/
					send(hwid, value, date);
					position++;
				} else if (ferror(rLogFile)) {
					printf("[gLog] Erreur lors de la lecture du fichier de log.\n");
				}
			}

			fseek(stateFile, 0, SEEK_SET);
			fprintf(stateFile, "%d\n", position);
			pthread_mutex_unlock(&mutex);
		}
        /* wait for next time */
        sleep(LOG_SEND_PERIOD);
    }

    return 0;
}

