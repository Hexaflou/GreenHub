/* Inclusions internes */
#include "ComponentInterface.h"
#include "Utility.h"
#include "Sensor.h"
#include "Actuator.h"
#include "EEP.h"
#include "SimulationSensor.h"
#include "ComReceptorTask.h"
#include "SimulationReceptorEnOcean.h"
#include "Configuration.h"
#include "ComSunSpotTask.h"
#include "../lib/cJSON.h"

/* Inclusions externes */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue.h>

/* Déclaration de constantes */
#define CONFIG_CJSON_LENGTH 250

/***************************PRIVEE***********************/
/* Déclaration de variables */
static mqd_t smq;
static sem_t mutex_sensorList;
static sem_t mutex_actuatorList;
static Sensor* p_sensorList;
static Actuator* p_actuatorList;
static EEP* p_EEPList;
static int alreadyInitialized = 0;

/* Pour le mode simulation */
static mqd_t mqReceptor;

/************************PUBLIC***************************************/

/* Fonction lançant les deux connections d'écoute avec les périphériques EnOcean et SunSpot */
int ComponentInterfaceInit() {
    /* Objet cJSON permettant de récupérer les informations du fichier de configuration CONFIG_FILE */
    cJSON * root, * receptorIP, * receptorPort, *eepConfig, *sensorConfig, *actuatorConfig;
    char c;
    char txt[CONFIG_CJSON_LENGTH];
    char * receptorIPTxt, * receptorPortTxt, *eepConfigTxt, *sensorConfigTxt, *actuatorConfigTxt;
    int nbOpenedAccolade;
    FILE *f;

    if (alreadyInitialized) {
        return ERROR;
    }

    /* Ouverture du fichier en lecture */
    f = fopen("configuration.txt", "r");

    if (f == NULL) {
        printf("[ComponentInterface] Erreur dans l'ouverture du fichier de configuration configuration.txt. \n");
        return ERROR;
    }
    /* Recuperation des informations de chaque capteur */
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
            /* Receptor IP */
            receptorIP = cJSON_GetObjectItem(root, "receptorIP");
            if (receptorIP == NULL) {
                printf("[ComponentInterfaceInit] IP du récepteur invalide dans le fichier configuration.txt\n");
                return ERROR;
            }
            receptorIPTxt = receptorIP->valuestring;

            /* Receptor Port */
            receptorPort = cJSON_GetObjectItem(root, "receptorPort");
            if (receptorPort == NULL) {
                printf("[ComponentInterfaceInit] Port du récepteur incorrect dans le fichier configuration.txt\n");
                return ERROR;
            }
            receptorPortTxt = receptorPort->valueint;

            /* Fichier de config. EEP */
            eepConfig = cJSON_GetObjectItem(root, "eepConfig");
            if (eepConfig == NULL) {
                printf("[ComponentInterfaceInit] Fichier de configuration EEP incorrect dans le fichier configuration.txt\n");
                return ERROR;
            }
            eepConfigTxt = eepConfig->valuestring;

            /* Fichier de config. Capteur */
            sensorConfig = cJSON_GetObjectItem(root, "sensorConfig");
            if (sensorConfig == NULL) {
                printf("[ComponentInterfaceInit] Fichier de configuration Sensor incorrect dans le fichier configuration.txt\n");
                return ERROR;
            }
            sensorConfigTxt = sensorConfig->valuestring;

            /* Fichier de config. Actionneur */
            actuatorConfig = cJSON_GetObjectItem(root, "actuatorConfig");
            if (actuatorConfig == NULL) {
                printf("[ComponentInterfaceInit] Fichier de configuration Actuator incorrect dans le fichier configuration.txt\n");
                return ERROR;
            }
            actuatorConfigTxt = actuatorConfig->valuestring;
        }
    }


    /* Création des mutex pour la liste de capteurs et la liste d'actionneurs */
    if (sem_init(&mutex_sensorList, 0, 1) == ERROR) {
        perror("[ComponentInterface] Erreur dans l initialisation du semaphore pour la liste de capteurs.\n");
        return ERROR;
    }
    if (sem_init(&mutex_actuatorList, 0, 1) == ERROR) {
        perror("[ComponentInterface] Erreur dans l initialisation du semaphore pour la liste d actionneurs.\n");
        return ERROR;
    }

    /* Initialisation des listes de capteurs et d'actionneurs */
    sem_wait(&mutex_sensorList);
    p_sensorList = NULL;
    p_actuatorList = NULL;
    p_EEPList = (EEP*) malloc(sizeof (EEP));
    p_EEPList->next = NULL;

    /* Chargement des capteurs et EEP */
    if (readConfig(sensorConfigTxt, eepConfigTxt, actuatorConfigTxt, &p_sensorList, &p_actuatorList, p_EEPList) == ERROR)
        return ERROR;
    sem_post(&mutex_sensorList);

    /* Mode Simulation (récepteur EnOcean, capteurs et actionneurs) */
    {
        mqReceptor = comSimulationReceptorTaskInit();
        StartSimulationSensor(mqReceptor);
    }

    /* Création et lancement des deux tâches permettant de communiquer avec le récepteur EnOcean */
    smq = comReceptorTaskInit();

    /* Lancement de 2 threads pour SunSPOTs et pour EnOcean */
    /*iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);*/

    cJSON_Delete(root);
    alreadyInitialized = 1;

    return OK;
}

/* Destruction des composants et des tâches */
int ComponentInterfaceClose() {
    comReceptorTaskClose();
    comSunSpotTaskClose();
    comSimulationReceptorTaskClose();
    StopSimulationSensor();
    destroyEEPList(p_EEPList); /* Désalloue p_EEPList */
    destroyComponentList(p_sensorList, p_actuatorList); /* Désalloue p_EEPList */
    return 0;
}

/* 
 * Fonction traitant les messages EnOcean.
 * Si le capteur emettant le message est enregistre dans notre configuration
 * son message sera traite. Si le message est un message de Teach-In ce capteur pourra
 * etre integre dynamiquement a notre configuration, seulement si son EEP est connu de
 * l'application.
 */
void ManageMessage(char* message) {
    Sensor* currentSensor;
    char * sensorRealId, * messageId;
    messageId = str_sub(message, 10, 17);
#if DEBUG > 0
    printf("Message : %s \n", message);
#endif

    sem_wait(&mutex_sensorList);
    currentSensor = p_sensorList;
    sensorRealId = str_sub(currentSensor->id, 0, 7);

    /* Recherche du capteur dans la liste de capteurs */
    while (currentSensor != NULL) {
        if (strcmp(sensorRealId, messageId) == 0) /* Détecteur présent dans la liste */ {
            /*printf("Détecteur présent dans la liste ! ID : %s \n", sensorRealId);*/
            currentSensor->decodeMessage(message, currentSensor);
        }
        currentSensor = currentSensor->next;
        sensorRealId = str_sub(currentSensor->id, 0, 7);
    }
    sem_post(&mutex_sensorList);

    free(sensorRealId);
    free(messageId);
    /* If the sensor isn't in the sensors' list */

}

/*****************************************************
 *
 *		API CAPTEUR/ACTIONNEUR
 * 
 ****************************************************/

/*
 * Permet d ajouter un capteur a partir de son EEP et son id
 * Sortie : Renvoie 0 si tout s'est bien passe, -1 si l EEP n est pas supporte,-2 si l EEP est inconnu et -3 si l'ID est déjà présent.
 */
int AddComponent(char* id, char org[2], char funct[2], char type[2]) {
    int result;
    if (strlen(id) > 8) {
        Sensor* currentSensor;
        sem_wait(&mutex_sensorList);
        currentSensor = p_sensorList;
        while (currentSensor != NULL) {
            if (strcmp(currentSensor->id, id) == 0) {
                sem_post(&mutex_sensorList);
                return COMPONENT_ALREADY_EXIST;
            }
            currentSensor = currentSensor->next;
        }
        result = AddComponentByEEP(id, (void**) & p_sensorList, p_EEPList, org, funct, type);
        sem_post(&mutex_sensorList);
    } else {
        Actuator* currentActuator;
        sem_wait(&mutex_actuatorList);
        currentActuator = p_actuatorList;
        while (currentActuator != NULL) {
            if (strcmp(currentActuator->id, id) == 0) {
                sem_post(&mutex_actuatorList);
                return COMPONENT_ALREADY_EXIST;
            }
            currentActuator = currentActuator->next;
        }
        result = AddComponentByEEP(id, (void**) & p_sensorList, p_EEPList, org, funct, type);
        sem_post(&mutex_actuatorList);
    }
    return result;
}


/*************** CAPTEUR ***************/

/*
 * Renvoie la liste de capteurs.
 */
Sensor * getSensorList() {
    return p_sensorList;
}

/*
 * Renvoie le mutex protegeant la liste de capteurs
 */
sem_t getSemSensor() {
    return mutex_sensorList;
}

/* 
 * Fonction permettant de retirer la valeur d un capteur. 
 * Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
 * Renvoie -1 si erreur, 0 si OK.
 */
int GetInfoFromSensor(char id[10], float * p_value) {
    char* realId;
    Sensor* currentSensor;
    realId = str_sub(id, 0, 7); /* L'ID reel d un peripherique est compose de 8 caracteres */
    *p_value = 0;

    sem_wait(&mutex_sensorList);
    currentSensor = p_sensorList;

    /* Recherche du capteur dans la liste de capteurs */
    while (currentSensor != NULL) {
        if (strcmp(currentSensor->id, realId) == 0) {
            /*printf("Détecteur présent dans la liste ! \n");*/
            sem_post(&mutex_sensorList);
            *p_value = currentSensor->value;
            return OK;
        } else {
            currentSensor = currentSensor->next;
        }
    }
    sem_post(&mutex_sensorList);
    free(realId);
    return ERROR;
}


/*************** ACTIONNEUR ***************/

/*
 * Renvoie la liste d'actionneurs.
 */
Actuator * getActuatorList() {
    return p_actuatorList;
}

/*
 * Renvoie le mutex protegeant la liste de capteurs
 */
sem_t getSemActuator() {
    return mutex_actuatorList;
}

/* 
 * Fonction permettant de retirer l etat d un actionneur. 
 * Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
 * Renvoie -1 si erreur, 0 si OK.
 */
int GetStatusFromActuator(char* id, float * p_value) {
    Actuator* p_currentActuator;
    if (strlen(id) != 10) {
        return INVALID_ID;
    }

    *p_value = 0;

    sem_wait(&mutex_actuatorList);
    p_currentActuator = p_actuatorList;

    /* Recherche de l'actionneur dans la liste d'actionneurs */
    while (p_currentActuator != NULL) {
        if (strcmp(p_currentActuator->id, id) == 0) {
            /*printf("Actionneur présent dans la liste ! \n");*/
            sem_post(&mutex_actuatorList);
            *p_value = p_currentActuator->status;
            return OK;
        } else {
            p_currentActuator = p_currentActuator->next;
        }
    }
    sem_post(&mutex_actuatorList);
    return ERROR;
}

/* 
 * Agit sur un actionneur de notre liste d actionneurs.
 * Entrees :
 * 	id : id de notre actionneur.
 * 	value : la valeur a donne dans l action
 */
int ActionActuator(char* id, float value) {
    Actuator* p_currentActuator;
    sem_wait(&mutex_actuatorList);
    p_currentActuator = p_actuatorList;

    /* Recherche de l'actionneur dans la liste d'actionneurs */
    while (p_currentActuator != NULL) {
        if (strcmp(p_currentActuator->id, id) == 0) {
            /*printf("Actionneur présent dans la liste ! \n");*/
            sem_post(&mutex_actuatorList);
            p_currentActuator->action(value, p_currentActuator, smq);
            return OK;
        } else {
            p_currentActuator = p_currentActuator->next;
        }
    }
    sem_post(&mutex_actuatorList);
    return ERROR;
}
