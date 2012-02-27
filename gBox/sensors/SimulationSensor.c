/* Inclusions internes */
#include "SimulationSensor.h"
#include "ComponentInterface.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Utility.h"
#include "ComReceptorTask.h"
#include <../../libs/gMemory/gMemory.h>

/* Inclusions externes */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <mqueue.h>


/***************************PRIVATE DECLARATION***********************/
static int launchSimulationSensor(char * id, int sleepingTime, SimuThreadList ** pp_simuThreadList, enum SIMU_TYPE simu_type);
static void *SimulationSensorTemp(void *ptr);
static void *SimulationSensorContact(void *ptr);
static void *SimulationSensorSwitch(void *ptr);
static void *SimulationSensorLight(void *ptr);
static void *SimulationSensorOccupancy(void *ptr);
static void *SimulationSensorLightOccupancy(void *ptr);

static char* CalculateCheckSum(char * message);

static mqd_t smq;
static SimuThreadList* p_simuThreadList;

/************************PUBLIC***************************************/

/* Fonction permettant de lancer la simulation de plusieurs capteurs. */
int StartSimulationSensor(mqd_t arg_smq) {
    smq = arg_smq;
    p_simuThreadList = NULL;
    srand(time(NULL));

    /*************************** SALON ***************************/
    /* Capteur de température entre 0 et 40°C : salon */
    launchSimulationSensor("00893360", 10, &p_simuThreadList, SIMU_TEMP);

    /* Capteur de luminosité entre 0 et 510Lux : salon */
    launchSimulationSensor("00054122", 3, &p_simuThreadList, SIMU_LIGHT);

    /* Capteur de contact : fenêtre salon */
    launchSimulationSensor("0001B015", 6, &p_simuThreadList, SIMU_CONTACT);

    /* Interrupteur : grande lumière salon */
    launchSimulationSensor("0021CBE5", 2, &p_simuThreadList, SIMU_SWITCH);

    /*************************** SALLE DE BAIN ***************************/
    /* Capteur de température entre 0 et 40°C : salle de bain */
    launchSimulationSensor("FFEA0321", 10, &p_simuThreadList, SIMU_TEMP);

    /* Capteur de luminosité (entre 0 et 510Lux) et de présence : salle de bain */
    launchSimulationSensor("00054155", 6, &p_simuThreadList, SIMU_LIGHT_OCCUPANCY);

    return 0;
}

int StopSimulationSensor() {
    SimuThreadList* p_simuThreadCurrent, *p_simuThreadDelete;
    p_simuThreadCurrent = p_simuThreadList;
    p_simuThreadDelete = p_simuThreadList;
    while (p_simuThreadCurrent != NULL) {
        p_simuThreadCurrent = p_simuThreadDelete->next;
        gfree(p_simuThreadDelete);
        p_simuThreadDelete = p_simuThreadCurrent;
    }
    return 0;
}

/************************PRIVATE***************************************/

int launchSimulationSensor(char * id, int sleepingTime, SimuThreadList ** pp_simuThreadList, enum SIMU_TYPE simu_type) {
    SimuThreadList* p_simuThread;
    void *(*SimulationSensorFunction)(void *ptr);
    ArgSensor * argSensor = (ArgSensor*) gmalloc(sizeof (ArgSensor));
    strncpy(argSensor->id, id, 9);
    argSensor->sleepingTime = sleepingTime;

    switch (simu_type) {
        case SIMU_TEMP:
            SimulationSensorFunction = SimulationSensorTemp;
            break;
        case SIMU_SWITCH:
            SimulationSensorFunction = SimulationSensorSwitch;
            break;
        case SIMU_CONTACT:
            SimulationSensorFunction = SimulationSensorContact;
            break;
        case SIMU_LIGHT:
            SimulationSensorFunction = SimulationSensorLight;
            break;
        case SIMU_OCCUPANCY:
            SimulationSensorFunction = SimulationSensorOccupancy;
            break;
        case SIMU_LIGHT_OCCUPANCY:
            SimulationSensorFunction = SimulationSensorLightOccupancy;
            break;
        default:
            printf("[launchSimulationSensor] Capteur non supporté pour la simulation.\n");
            return ERROR;
    }

    if (*pp_simuThreadList == NULL) { /* Liste vide */
        /* Creation du premier de la liste */
        *pp_simuThreadList = (SimuThreadList*) gmalloc(sizeof (SimuThreadList));
        (*pp_simuThreadList)->next = NULL;
        p_simuThread = *pp_simuThreadList;
    } else {
        p_simuThread = *pp_simuThreadList;

        /* Parcours de la liste jusqu a l'avant dernier élément de la liste */
        while ((p_simuThread != NULL) && (p_simuThread->next != NULL)) {
            p_simuThread = p_simuThread->next;
        }
        /* Creation du capteur en fin de liste */
        p_simuThread->next = (SimuThreadList*) gmalloc(sizeof (SimuThreadList));
        p_simuThread = p_simuThread->next;
        p_simuThread->next = NULL;
    }
    pthread_create(&(p_simuThread->thread), NULL, SimulationSensorFunction, (void*) argSensor);

    return OK;
}

/* Tâche simulant un capteur de température, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * Etant dans un contexte de simulation, ce capteur mesure une température en 0 et 40°C.
 * p_argSensor : Structure argument contenant les informations ID du capteur, et le temps entre chaque envoi de message.
 */
void * SimulationSensorTemp(void * p_argSensor) {
    char message[29];
    int sleepingTime;
    float temp, multiplier;
    char tempHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    /*
     * Les valeurs de température émises par un capteur doivent être décodés en calculant un multiplicateur.
     * D'après la documentation EnOcean multiplier = (scaleMax-scaleMin)/(rangeMax-rangeMin)
     * Ici scaleMax = 40; scaleMin = 0; rangeMax = 0; rangeMin = 255
     */
    multiplier = ((float) 40) / ((float) - 255);

    /*	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
    while (1) {
        sleep(sleepingTime);
        temp = rand() % 40 + 1;

        /* Dans ce cas présent multiplier < 0 donc on applique une formule tirée de la documentation EnOcean */
        temp = (temp - 40) / multiplier;

        sprintf(tempHexa, "%X", (int) temp);

        /* Si la temperature est exprime sur un caractere en hexa il va falloir l etaler sur nos deux caracteres hexa */
        if (temp < 16) {
            tempHexa[1] = tempHexa[0];
            tempHexa[0] = '0';
            tempHexa[2] = '\0';
        }
        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, "0000"); /* BYTE 3 à 2 */
        strcat(message, tempHexa); /* BYTE 1 */
        strcat(message, "00"); /* BYTE 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "00"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*printf("Message du capteur de température : %s\n",message);*/


        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }

    return (void*) NULL;
}

/* Tâche simulant un capteur de contact, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID et EEP du capteur.
 */
void * SimulationSensorContact(void * p_argSensor) {
    char message[29];
    int contact, sleepingTime;
    char contactHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    /*AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
    while (1) {
        sleep(sleepingTime);
        contact = rand() % 2;
        sprintf(contactHexa, "%X", contact);
        contactHexa[1] = contactHexa[0];
        contactHexa[0] = '0';
        contactHexa[2] = '\0';

        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, "000000"); /* BYTE 3 à 1 */
        strcat(message, contactHexa); /* BYTE 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "00"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*printf("Message du capteur de contact : %s\n",message);*/

        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }
    return (void*) NULL;
}

/* Tâche simulant un interrupteur à 4 etats, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID du capteur.
 */
void * SimulationSensorSwitch(void * p_argSensor) {
    char message[29];
    int i_switch, sleepingTime;
    char switchHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    /*	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
    while (1) {
        sleep(sleepingTime);
        i_switch = (rand() % 4) << 1;
        sprintf(switchHexa, "%X", i_switch);
        switchHexa[1] = '0';
        switchHexa[2] = '\0';

        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, switchHexa); /* BYTE 3 */
        strcat(message, "000000"); /* BYTE 2 à 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "30"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*	printf("Message du capteur d'interrupteur : %s\n",message);*/

        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }
    return (void*) NULL;
}

/* Tâche simulant un capteur de luminosité, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * Etant dans un contexte de simulation, ce capteur mesure une luminosité en 0 et 510lx.
 * p_argSensor : Structure argument contenant les informations ID du capteur, et le temps entre chaque envoi de message.
 */
void * SimulationSensorLight(void * p_argSensor) {
    char message[29];
    int sleepingTime;
    float light, multiplier;
    char lightHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    /*
     * Les valeurs de luminosité émises par un capteur doivent être décodés en calculant un multiplicateur.
     * D'après la documentation EnOcean multiplier = (scaleMax-scaleMin)/(rangeMax-rangeMin)
     * Ici scaleMax = 510; scaleMin = 0; rangeMax = 255; rangeMin = 0
     */
    multiplier = ((float) 510) / ((float) 255);

    while (1) {
        sleep(sleepingTime);
        light = rand() % 510 + 1;

        /* Dans ce cas présent multiplier > 0 donc on applique une formule tirée de la documentation EnOcean */
        light = light / multiplier;

        sprintf(lightHexa, "%X", (int) light);

        /* Si la luminosité est exprimé sur un caractère en hexa il va falloir l'étaler sur nos deux caractères hexa */
        if (light < 16) {
            lightHexa[1] = lightHexa[0];
            lightHexa[0] = '0';
            lightHexa[2] = '\0';
        }
        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, "00"); /* BYTE 3 */
        strcat(message, lightHexa); /* BYTE 2 */
        strcat(message, "0000"); /* BYTE 1 à 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "00"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*		printf("Message du capteur de température : %s\n",message);*/


        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }
    return (void*) NULL;
}

/* Tâche simulant un capteur de présence, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID du capteur.
 */
void * SimulationSensorOccupancy(void * p_argSensor) {
    char message[29];
    int occupancy, sleepingTime;
    char occupancyHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    while (1) {
        sleep(sleepingTime);
        occupancy = (rand() % 2) << 1;
        sprintf(occupancyHexa, "%X", occupancy);
        occupancyHexa[1] = occupancyHexa[0];
        occupancyHexa[0] = '0';
        occupancyHexa[2] = '\0';

        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, "000000"); /* BYTE 3 à 1 */
        strcat(message, occupancyHexa); /* BYTE 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "00"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*printf("Message du capteur de présence : %s\n",message);*/

        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }
    return (void*) NULL;
}

/* Tâche simulant un capteur de luminosité et de présence, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * La luminosité a un intervalle de 0 à 510Lux.
 * p_argSensor : Structure argument contenant les informations ID du capteur.
 */
void * SimulationSensorLightOccupancy(void * p_argSensor) {
    char message[29];
    int occupancy, sleepingTime;
    float light, multiplier;
    char occupancyHexa[3], lightHexa[3];
    char id[9];

    sleepingTime = ((struct ArgSensor*) p_argSensor)->sleepingTime;
    strcpy(id, ((struct ArgSensor*) p_argSensor)->id);
    gfree(p_argSensor);

    /*
     * Les valeurs de luminosité émises par un capteur doivent être décodés en calculant un multiplicateur.
     * D'après la documentation EnOcean multiplier = (scaleMax-scaleMin)/(rangeMax-rangeMin)
     * Ici scaleMax = 510; scaleMin = 0; rangeMax = 255; rangeMin = 0
     */
    multiplier = ((float) 510) / ((float) 255);

    while (1) {
        sleep(sleepingTime);
        occupancy = (rand() % 2) << 1;
        sprintf(occupancyHexa, "%X", occupancy);
        occupancyHexa[1] = occupancyHexa[0];
        occupancyHexa[0] = '0';
        occupancyHexa[2] = '\0';

        light = rand() % 510 + 1;

        /* Dans ce cas présent multiplier > 0 donc on applique une formule tirée de la documentation EnOcean */
        light = light / multiplier;

        sprintf(lightHexa, "%X", (int) light);

        /* Si la luminosité est exprimé sur un caractère en hexa il va falloir l'étaler sur nos deux caractères hexa */
        if (light < 16) {
            lightHexa[1] = lightHexa[0];
            lightHexa[0] = '0';
            lightHexa[2] = '\0';
        }

        /******* HEADER *******/
        strcpy(message, "A55A"); /* Début d'un message */
        strcat(message, "0B"); /* SYNC_BYTE */
        strcat(message, "07"); /* LENGTH_BYTE */
        /***** FIN HEADER *****/

        /******* DATA BYTE *******/
        strcat(message, "00"); /* BYTE 3 */
        strcat(message, lightHexa); /* BYTE 2 */
        strcat(message, "00"); /* BYTE 1 */
        strcat(message, occupancyHexa); /* BYTE 0 */
        /***** FIN DATA BYTE *****/

        /******* ID *******/
        strcat(message, id); /* 4 octets */
        /***** FIN ID *****/

        strcat(message, "00"); /* STATUS BYTE */
        strcat(message, "00"); /* CHECKSUM */

        /*printf("Message du capteur de luminosité et de présence : %s\n",message);*/

        mq_send(smq, message, MAX_MQ_SIZE, 0);
    }
    return (void*) NULL;
}

/*
 * Fonction permettant de calculer la checksum d'un message. Le champ checksum du message en paramètre sera mis à jour.
 * Paramètre d'entrée :char * message : trame à traiter.
 */
char* CalculateCheckSum(char * message) {
    int nbChar, ii;
    int byteSum;
    char byte[2];
    char * byteSumHexa;
    byteSumHexa = (char*) gmalloc(sizeof (char) *30);
    nbChar = xtoi(str_sub(message, 0, 1));
    for (ii = 0; ii < nbChar; ii += 2) {
        /* TODO: MARCHE PAS */
        byte[0] = message[ii];
        byte[1] = message[ii + 1];
        byteSum = byteSum + xtoi(byte);
    }
    sprintf(byteSumHexa, "%X", byteSum);
    printf("ByteSumHexa : %s \n", byteSumHexa);
    gfree(byteSumHexa);
    return byteSumHexa;
}
