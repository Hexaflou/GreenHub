#include "realtime.h"
#include <semaphore.h>
#include "gCommunication/gCommunication.h"
#include "sensors/ComponentInterface.h"
#include "sensors/Sensor.h"
#include <pthread.h>

/********************* LOCAL functions and variables *****************/

static int lPeriod = 0;
static pthread_t periodLog;
static sem_t semSensorList;

void * gRealTimeLog(void * attr);
/* initialise la tache de log periodique des donnees*/
int gRTInit(int period);
int gRTClose();


/******************** PUBLIC functions *******************************/

/* change la frequence de log du mode temps reel */
int gRTSetPeriod(int period) {

    if (period == 0 && lPeriod != 0) {
        gRTClose();
    } else if (lPeriod == 0 && period != 0) {
        gRTInit(period);
    } else {
        lPeriod = period;
    }

    return 0;
}

/* initialise la tache de log periodique des donnees*/
int gRTInit(int period) {
    lPeriod = period;
    return pthread_create(&periodLog, NULL, gRealTimeLog, (void *) NULL);
}

/* Kill de la tâche de log periodique des donnees */
int gRTClose() {
    pthread_cancel(periodLog);
    return 0;
}

/**********************************************************************/

void * gRealTimeLog(void * attr) {
    Sensor* tempSensor = NULL;

    tempSensor = getSensorList();
    semSensorList = getSemSensor();



    while (lPeriod != 0) {


        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

        sem_wait(&semSensorList);

        while (tempSensor != NULL) {
            tempSensor->id[11] = '\0';
            gCommunicationSendValue(tempSensor->id, tempSensor->value);
            tempSensor = tempSensor->next;
        }


        sem_post(&semSensorList);

        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

        sleep(lPeriod);

        tempSensor = getSensorList();
    }

    return (void *) NULL;
}


