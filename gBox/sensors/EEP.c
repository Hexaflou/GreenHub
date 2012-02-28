/* Inclusions internes */
#include "Sensor.h"
#include "Actuator.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "string.h"
#include <stdlib.h>
#include "../lib/cJSON.h"
#include "Utility.h"

/* Inclusions externes */
#include <stdio.h>

/* Déclaration de constantes*/
#define TAILLE_EEP 8
#define TAILLE_NAME 70
#define TAILLE_RANGE 200

/*
 * Initialise la liste des EEP à partir d'un fichier .txt de configuration contenant les informations des capteurs : EEP et nom
 */
int initializeEEPList(char* fileNameEEP, EEP* EEPList) {
    struct EEP* EEPCurrent;
    char eep[TAILLE_EEP + TAILLE_NAME + TAILLE_RANGE + 30];
    cJSON* root, *rangeData;
    int funct, org;
    char * functChar, *orgChar;
    char c;
    int nbOpenedAccolade = 0;

    /* Ouverture du fichier en lecture */
    FILE *f = fopen(fileNameEEP, "r");

    EEPCurrent = EEPList;
    /* Pour chaque "eep" dans le fichier */
    c = fgetc(f);
    while (c != EOF) {
        memset(eep, 0, sizeof (eep));
        /* Lecture jusqu'au début de l'objet json :*/
        while (c != '{' && c != EOF) {
            c = fgetc(f);
        }
        if (c == EOF)
            break;
        sprintf(eep, "%s%c", eep, c);

        /* Lecture de l'objet JSON : */
        nbOpenedAccolade = 1;
        while (nbOpenedAccolade > 0) {
            c = fgetc(f);
            sprintf(eep, "%s%c", eep, c);
            if (c == '{') {
                nbOpenedAccolade++;
            } else if (c == '}') {
                nbOpenedAccolade--;
            }
        }
        /* Récupération des données EEP et name*/
        root = cJSON_Parse(eep);
        strcpy(EEPCurrent->eep, cJSON_GetObjectItem(root, "EEP")->valuestring);
        strcpy(EEPCurrent->name, cJSON_GetObjectItem(root, "Name")->valuestring);
        rangeData = cJSON_GetObjectItem(root, "RangeData");

        /* Création des données org, funct */
        orgChar = str_sub(EEPCurrent->eep, 0, 1);
        functChar = str_sub(EEPCurrent->eep, 2, 3);
        org = xtoi(orgChar);
        funct = xtoi(functChar);

        if (rangeData != NULL) {
            cJSON * range;
            EEPCurrent->scaleMin = cJSON_GetObjectItem(rangeData, "scaleMin")->valueint;
            EEPCurrent->scaleMax = cJSON_GetObjectItem(rangeData, "scaleMax")->valueint;
            range = cJSON_GetObjectItem(rangeData, "scaleMin");
            if (range != NULL) {
                EEPCurrent->rangeMin = cJSON_GetObjectItem(rangeData, "rangeMin")->valueint;
                EEPCurrent->rangeMax = cJSON_GetObjectItem(rangeData, "rangeMax")->valueint;
            }


        }

        /* Récupération de la fonction et des arguments associés */
        switch (org) {
            case 5:
            {
                switch (funct) {
                    case 2:
                    {
                        EEPCurrent->AddComponent = AddSensorSwitch;
                        break;
                    }
                    case 3:
                    case 4:
                    case 10:
                    {
                        EEPCurrent->AddComponent = NULL;
                        break;
                    }

                } /* Fin switch funct */
                break;
            } /* Fin case 5 */
            case 6:
            {
                EEPCurrent->AddComponent = AddSensorContact;
                break;
            } /* Fin case 6 */
            case 7:
            {
                switch (funct) {
                    case 2:
                    {
                        EEPCurrent->AddComponent = AddSensorTemp;
                        break;
                    } /* Fin case 2 */
                    case 8:
                    {
                        EEPCurrent->AddComponent = AddSensorLightOccupancy;
                        break;
                    }
                    default:
                    {
                        EEPCurrent->AddComponent = NULL;
                        break;
                    } /* Fin default */
                } /* Fin switch funct*/
                break;
            } /* Fin case 7 */
            case 255:
            { /* Capteurs SunSpot */
                switch (funct) {
                    case 255:
                    {
                        EEPCurrent->AddComponent = AddSensorTempLightSunSpot;
                        break;
                    }
                }
            }
            case 238:
            { /* EE : Actionneurs */
                switch (funct) {
                    case 238:
                    { /* EE : Current Actuator */
                        EEPCurrent->AddComponent = AddActuatorCurrent;
                        break;
                    }
                    case 239:
                    { /* EF : Thermostat actuator */
                        EEPCurrent->AddComponent = AddActuatorTemp;
                        break;
                    }
                }
            }
        } /* Fin switch org */
        c = fgetc(f);
        if (c != EOF) {
            EEPCurrent->next = (EEP*) malloc(sizeof (EEP));
            EEPCurrent = EEPCurrent->next;
	    EEPCurrent->next = NULL;
        }
        free(orgChar);
        free(functChar);
        cJSON_Delete(root);
    } /* Fin while */
    /* Fermeture du fichier */
    fclose(f);

    return 0;
}

/*
 * Fonction retournant le cJSON créé a partir des parametres EEP et name
 * L'objet renvoyé devra être supprimé par l'intermédiaire de la méthode
 * cJSON_Delete()
 */
cJSON* createCJSONEEP(char* EEP, char* name) {

    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "EEP", EEP);
    cJSON_AddStringToObject(root, "Name", name);
    return root;
}

/*
 * Fonction permettant l'écriture dans un fichier de l'EEPList
 */
int writeEEPList(char* fileNameEEP, EEP* p_EEPList) {
    struct EEP* p_EEPCurrent;
    cJSON* root;
    char* EEPstr;

    /* Ouverture du fichier en ecriture */
    FILE *f = fopen(fileNameEEP, "w");

    if (p_EEPList != NULL) {
        p_EEPCurrent = p_EEPList;
        /* Parcours de la EEPList */
        while (p_EEPCurrent != NULL) {
            /* Creation du cJSON */
            root = createCJSONEEP(p_EEPCurrent->eep, p_EEPCurrent->name);
            EEPstr = cJSON_Print(root);
            /* Ecriture de l EEP */
            fprintf(f, "%s", EEPstr);

            cJSON_Delete(root);

            /* Passage au suivant */
            p_EEPCurrent = p_EEPCurrent ->next;
        } /* Fin while */
    }
    /* Fermeture du fichier */
    fclose(f);
    return 0;
}

/*
 * Supprime la liste d'EEP passée en paramètres
 */
int destroyEEPList(EEP* p_EEPList) {
    EEP* p_EEPCurrent, *p_EEPDelete;
    p_EEPCurrent = p_EEPList;
    p_EEPDelete = p_EEPList;
    while (p_EEPCurrent != NULL) {
        p_EEPCurrent = p_EEPDelete->next;
        free(p_EEPDelete);
        p_EEPDelete = p_EEPCurrent;
    }
    return 0;
}

/*
 * Fonction permettant d'ajouter un composant(capteur/actionneur) a la liste de composants a partir de son EEP
 * L EEP est compose de 6 caractères, provenant de org-funct-type.
 * Renvoie 0 si OK, -1 si l EEP n est pas supporte, et -2 si l EEP correspondant est introuvable.
 */
int AddComponentByEEP(char* id, void ** pp_componentList, EEP* EEPList, char org[3], char funct[3], char type[3]) {
    char eep[7];

    /* Concaténation de org, funct et type dans eep */
    strcpy(eep, org);
    strcat(eep, funct);
    strcat(eep, type);

    while (EEPList != NULL) {
        if (strcmp(EEPList->eep, eep) != 0) /* Si l EEP courant est different de l EEP du capteur */ {
            EEPList = EEPList->next;
        } else {
            if (EEPList->AddComponent == NULL) {
                return NOT_SUPPORTED; /* L EEP n est pas encore supporte */
            }
            return EEPList->AddComponent(id, pp_componentList, eep, EEPList->scaleMin, EEPList->scaleMax, EEPList->rangeMin, EEPList->rangeMax);
        }
    }
    return NOT_FOUND; /* EEP introuvable */
}

/*
 * Ajoute un capteur de contact à la liste de capteurs
 */
int AddSensorContact(char* id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Sensor* p_sensor;
    if (*pp_sensorList == NULL) { /* Liste vide */
        /* Creation du premier de la liste */
        *pp_sensorList = (Sensor*) malloc(sizeof (Sensor));
        ((Sensor*) (*pp_sensorList))->next = NULL;
        p_sensor = (Sensor*) * pp_sensorList;
    } else {
        p_sensor = *pp_sensorList;
        while ((p_sensor != NULL) && (p_sensor->next != NULL)) { /* Parcours de la liste jusqu a l avant dernier*/

            p_sensor = p_sensor->next;
        }
        /* Creation du capteur en fin de liste */
        p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
        p_sensor = p_sensor->next;
        p_sensor->next = NULL;
    }
    /* Ajout des parametres du capteur */
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 'e';
    p_sensor->id[9] = 'C';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = NULL;
    p_sensor->decodeMessage = decodeMessageContact;
    p_sensor->next = NULL;
    return OK;
}

/*
 ** Ajoute un capteur swicth à la liste de capteurs
 **
 */

int AddSensorSwitch(char* id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {

    Sensor* p_sensor;
    if (*pp_sensorList == NULL) { /* Liste vide */
        /* Creation du premier de la liste */
        *pp_sensorList = (Sensor*) malloc(sizeof (Sensor));
        ((Sensor*) (*pp_sensorList))->next = NULL;
        p_sensor = ((Sensor*) * pp_sensorList);
    } else {
        p_sensor = *pp_sensorList;
        while ((p_sensor != NULL) && (p_sensor->next != NULL)) { /* Parcours de la liste jusqu a l avant dernier*/
            p_sensor = p_sensor->next;
        }
        /* Creation du capteur en fin de liste */
        p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
        p_sensor = p_sensor->next;
        p_sensor->next = NULL;
    }
    /* Ajout des parametres du capteur */
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 'e';
    p_sensor->id[9] = 'S';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = NULL;
    p_sensor->decodeMessage = decodeMessageSwitch;
    p_sensor->next = NULL;
    return OK;
}

/*
 ** Ajoute un capteur de temperature à la liste de capteurs
 **
 */
int AddSensorTemp(char* id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Sensor* p_sensor;
    if (*pp_sensorList == NULL) { /* Liste vide */
        /* Creation du premier de la liste */
        *pp_sensorList = (Sensor*) malloc(sizeof (Sensor));
        ((Sensor*) (*pp_sensorList))->next = NULL;
        p_sensor = ((Sensor*) * pp_sensorList);
    } else {
        p_sensor = *pp_sensorList;
        while ((p_sensor != NULL) && (p_sensor->next != NULL)) { /* Parcours de la liste jusqu a l avant dernier*/
            p_sensor = p_sensor->next;
        }
        /* Creation du capteur en fin de liste */
        p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
        p_sensor = p_sensor->next;
        p_sensor->next = NULL;
    }
    /* Ajout des parametres du capteur */
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 'e';
    p_sensor->id[9] = 'T';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = (SensorRange*) malloc(sizeof (SensorRange));
    p_sensor->rangeData->scaleMax = scaleMax;
    p_sensor->rangeData->scaleMin = scaleMin;
    p_sensor->rangeData->rangeMax = rangeMax;
    p_sensor->rangeData->rangeMin = rangeMin;
    p_sensor->decodeMessage = decodeMessageTemp;
    p_sensor->next = NULL;
    return OK;
}

/*
 ** Ajoute un capteur de presence et de luminosite à la liste de capteurs
 **
 */
int AddSensorLightOccupancy(char* id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Sensor* p_sensor;
    if (*pp_sensorList == NULL) { /* Liste vide */
        /* Creation du premier de la liste */
        *pp_sensorList = (Sensor*) malloc(sizeof (Sensor));
        ((Sensor*) (*pp_sensorList))->next = NULL;
        p_sensor = ((Sensor*) * pp_sensorList);
    } else {
        p_sensor = *pp_sensorList;
        while ((p_sensor != NULL) && (p_sensor->next != NULL)) { /* Parcours de la liste jusqu a l avant dernier*/
            p_sensor = p_sensor->next;
        }
        /* Creation du capteur en fin de liste */
        p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
        p_sensor = p_sensor->next;
        p_sensor->next = NULL;
    }
    /* Ajout des parametres du capteur de luminosite */
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 'e';
    p_sensor->id[9] = 'L';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = (SensorRange*) malloc(sizeof (SensorRange));
    p_sensor->rangeData->scaleMax = scaleMax;
    p_sensor->rangeData->scaleMin = scaleMin;
    p_sensor->rangeData->rangeMax = rangeMax;
    p_sensor->rangeData->rangeMin = rangeMin;
    p_sensor->decodeMessage = decodeMessageLight;

    /* Creation du capteur de presence en fin de liste */
    p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
    p_sensor = p_sensor->next;
    /* Ajout des parametres du capteit de presence */
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 'e';
    p_sensor->id[9] = 'O';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = NULL;
    p_sensor->decodeMessage = decodeMessageOccupancy;
    p_sensor->next = NULL;

    return OK;
}

int AddSensorTempLightSunSpot(char* id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Sensor* p_sensor;
    if (*pp_sensorList == NULL) {
        *pp_sensorList = (Sensor*) malloc(sizeof (Sensor));
        ((Sensor*) (*pp_sensorList))->next = NULL;
        p_sensor = ((Sensor*) * pp_sensorList);
    } else {
        p_sensor = *pp_sensorList;
        while ((p_sensor != NULL) && (p_sensor->next != NULL)) {
            p_sensor = p_sensor->next;
        }
        /* Creation du capteur en fin de liste */
        p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
        p_sensor = p_sensor->next;
        p_sensor->next = NULL;
    }
    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 's';
    p_sensor->id[9] = 'L';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->rangeData = NULL;
    p_sensor->decodeMessage = decodeMessageLight;

    p_sensor->next = (Sensor*) malloc(sizeof (Sensor));
    p_sensor = p_sensor->next;

    strncpy(p_sensor->id, id, 8);
    p_sensor->id[8] = 's';
    p_sensor->id[9] = 'T';
    p_sensor->id[10] = '\0';
    strncpy(p_sensor->EEP, eep, 7);
    p_sensor->value = 0;
    p_sensor->decodeMessage = decodeMessageTemp;
    p_sensor->next = NULL;
    return OK;
}

int AddActuatorCurrent(char* id, void ** pp_actuatorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Actuator* p_actuator;
    
    if (*pp_actuatorList == NULL) {
        *pp_actuatorList = (Actuator*) malloc(sizeof (Actuator));
        ((Actuator*) (*pp_actuatorList))->next = NULL;
        p_actuator = ((Actuator*) * pp_actuatorList);
    } else {
        p_actuator = *pp_actuatorList;
        while ((p_actuator != NULL) && (p_actuator->next != NULL)) {
            p_actuator = p_actuator->next;
        }
        /* Insertion de l'actionneur en fin de liste */
        p_actuator->next = (Actuator*) malloc(sizeof (Actuator));
        p_actuator = p_actuator->next;
        p_actuator->next = NULL;
    }

    strncpy(p_actuator->id, id, 12);
    strncpy(p_actuator->EEP, eep, 7);
    p_actuator->status = 0;
    p_actuator->rangeData = NULL;
    p_actuator->action = actionCurrent;
    p_actuator->next = NULL;
    return OK;
}

int AddActuatorTemp(char* id, void ** pp_actuatorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax) {
    Actuator* p_actuator;

    if (*pp_actuatorList == NULL) {
        *pp_actuatorList = (Actuator*) malloc(sizeof (Actuator));
        ((Actuator*) (*pp_actuatorList))->next = NULL;
        p_actuator = ((Actuator*) * pp_actuatorList);
    } else {
        p_actuator = *pp_actuatorList;
        while ((p_actuator != NULL) && (p_actuator->next != NULL)) {
            p_actuator = p_actuator->next;
        }
        /* Insertion de l'actionneur en fin de liste */
        p_actuator->next = (Actuator*) malloc(sizeof (Actuator));
        p_actuator = p_actuator->next;
        p_actuator->next = NULL;
    }

    strncpy(p_actuator->id, id, 12);
    strncpy(p_actuator->EEP, eep, 7);
    p_actuator->status = 0;
    p_actuator->rangeData = (ActuatorRange*) malloc(sizeof (ActuatorRange));
    p_actuator->rangeData->rangeMin = scaleMin;
    p_actuator->rangeData->rangeMax = scaleMax;
    p_actuator->action = actionTemp;
    p_actuator->next = NULL;
    return OK;
}
