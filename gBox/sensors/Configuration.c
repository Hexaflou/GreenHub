/* Inclusions internes */
#include "Sensor.h"
#include "Actuator.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "../lib/cJSON.h"

/* Inclusions externes */
#include <stdio.h>

/* Définitions de constantes */
#define TAILLE_ID_SENSOR 8
#define TAILLE_ID_ACTUATOR 13
#define TAILLE_EEP 6

/*
 * Retourne le cJSON contenant les donnees passees en parametre : l'id et l'EEP
 */
cJSON* createCSON(char* id, char* EEP) {
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "id", id);
	cJSON_AddStringToObject(root, "EEP", EEP);
	return root;
}

/*
 * Charge la configuration du systeme a partir de la lecture d'un fichier contenant l'EEP et l'id des capteurs
 */
int readConfig(char* fileNameSensor, char* fileNameEEP, char* fileNameActuator, Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList) {

	char sensor[TAILLE_ID_SENSOR + TAILLE_EEP + 40];
	char actuator[TAILLE_ID_ACTUATOR + TAILLE_EEP + 40];
	cJSON *root, *eep;
	char* org, *funct, *type;
	int c;
	char * id;
	char * eepstr = NULL;
	int nbOpenedAccolade;



	/* Ouverture du fichier en lecture */
	FILE *f = fopen(fileNameSensor, "r");

	if (f == NULL) {
		printf("[readConfig] Erreur dans l'ouverture du fichier de capteurs %s.\n", fileNameSensor);
		return ERROR;
	}
	/* Initialisation de la EEPList */
	initializeEEPList(fileNameEEP, EEPList);

	/* Recuperation des informations de chaque capteur */
	c = fgetc(f);
	while (c != EOF) {
		memset(sensor, 0, sizeof (sensor));
		/* Lecture jusqu'au debut de l'objet json :*/
		while (c != '{' && c != EOF) {
			c = fgetc(f);
		}
		if (c == EOF)
			break;
		sprintf(sensor, "%s%c", sensor, c);

		/* Lecture de l'objet JSON : */
		nbOpenedAccolade = 1;
		while (nbOpenedAccolade > 0) {
			c = fgetc(f);
			if (c == EOF)
				break;
			sprintf(sensor, "%s%c", sensor, c);
			if (c == '{') {
				nbOpenedAccolade++;
			} else if (c == '}') {
				nbOpenedAccolade--;
			}
		}
		if (c == EOF)
			break;
		sprintf(sensor, "%s%c", sensor, '\0');
		/* Recuperation des donnees du json */
		root = cJSON_Parse(sensor);
		if (root != NULL) {
			/* EEP */
			eep = cJSON_GetObjectItem(root, "EEP");
			if (eep == NULL) {
				printf("[readConfig] EEP invalide dans le fichier %s\n", fileNameSensor);
				return ERROR;
			}
			eepstr = eep->valuestring;
			/* org, funct, type */
			org = str_sub(eepstr, 0, 1);
			funct = str_sub(eepstr, 2, 3);
			type = str_sub(eepstr, 4, 5);
			/* id */
			id = cJSON_GetObjectItem(root, "id")->valuestring;
			if (id == NULL) {
				printf("[readConfig] ID invalide dans le fichier %s\n", fileNameSensor);
				return ERROR;
			}
			/* Ajout du capteur */
			AddComponentByEEP(id, (void**) pp_sensorList, EEPList, org, funct, type);
			cJSON_Delete(root);
			free(org);
			free(funct);
			free(type);
		}
		c = fgetc(f);
		} /* Fin while */
		/* Fermeture du fichier */
		fclose(f);

		/* Ouverture du fichier d actionneur */
		f = fopen(fileNameActuator, "r");
		if (f == NULL) {
			printf("[readConfig] Erreur dans l'ouverture du fichier d'actionneurs %s.\n", fileNameActuator);
		}

		/* Recuperation des informations de chaque actionneur */
		c = fgetc(f);
		while (c != EOF) {
			memset(actuator, 0, sizeof (actuator));
			/* Lecture jusqu'au debut de l'objet json :*/
			while (c != '{' && c != EOF) {
				c = fgetc(f);
			}
			if (c == EOF) {
				break;
			}
			sprintf(actuator, "%s%c", actuator, c);

			/* Lecture de l'objet JSON : */
			nbOpenedAccolade = 1;
			while (nbOpenedAccolade > 0) {
				c = fgetc(f);
				if (c == EOF)
					break;
				sprintf(actuator, "%s%c", actuator, c);
				if (c == '{') {
					nbOpenedAccolade++;
				} else if (c == '}') {
					nbOpenedAccolade--;
				}
			}
			if (c == EOF)
				break;
			sprintf(actuator, "%s%c", actuator, '\0');
			/* Recuperation des donnees du json */
			root = cJSON_Parse(actuator);
			if (root != NULL) {
				/* EEP */
				eep = cJSON_GetObjectItem(root, "EEP");
				if (eep == NULL) {
					printf("[readConfig] EEP invalide dans le fichier %s\n", fileNameActuator);
					return ERROR;
				}
				eepstr = eep->valuestring;
				/* org, funct, type */
				org = str_sub(eepstr, 0, 1);
				funct = str_sub(eepstr, 2, 3);
				type = str_sub(eepstr, 4, 5);
				/* id */
				id = cJSON_GetObjectItem(root, "id")->valuestring;
				if (id == NULL) {
					printf("[readConfig] ID invalide dans le fichier %s\n", fileNameActuator);
					return ERROR;
				}
				/* Ajout du capteur */
				AddComponentByEEP(id, (void**) pp_actuatorList, EEPList, org, funct, type);
				cJSON_Delete(root);
				free(org);
				free(funct);
				free(type);
			}
			c = fgetc(f);
			} /* Fin while */
			/* Fermeture du fichier */
			fclose(f);
			return OK;
		}

		/*
		 * Détruit la liste de composants (actionneurs/capteurs).
		 */
		int destroyComponentList(Sensor* p_sensorList, Actuator* p_actuatorList) {
			Sensor* p_sensorCurrent, *p_sensorDelete;
			Actuator* p_actuatorCurrent, *p_actuatorDelete;
			sem_t semSensorList, semActuatorList;
			semSensorList = getSemSensor();
			semActuatorList = getSemActuator();

			sem_wait(&semSensorList);
			p_sensorCurrent = p_sensorList;
			p_sensorDelete = p_sensorList;
			while (p_sensorCurrent != NULL) {
				p_sensorCurrent = p_sensorDelete->next;
				if (p_sensorDelete->rangeData != NULL)
					free(p_sensorDelete->rangeData);
				free(p_sensorDelete);
				p_sensorDelete = p_sensorCurrent;
			}
			sem_post(&semSensorList);

			sem_wait(&semActuatorList);
			p_actuatorCurrent = p_actuatorList;
			p_actuatorDelete = p_actuatorList;
			while (p_actuatorCurrent != NULL) {
				p_actuatorCurrent = p_actuatorDelete->next;
				if (p_actuatorDelete->rangeData != NULL)
					free(p_actuatorDelete->rangeData);
				free(p_actuatorDelete);
				p_actuatorDelete = p_actuatorCurrent;
			}
			sem_post(&semActuatorList);
			return 0;

		}

		/*
		 * Enregistre dans la configuration du systeme la liste des capteurs
		 */
		void writeAllSensorConfig(char* fileNameSensor, Sensor * p_sensorList){
			Sensor * pCurrent;
			cJSON * root;
			char * sensor, *id, *eep;
			FILE *f;

			remove (fileNameSensor);
			/* Ouverture du fichier en ecriture */
			f = fopen(fileNameSensor, "w");


			if (p_sensorList != NULL) {
				pCurrent = p_sensorList;
				/* Parcours de la liste de capteurs */
				while (pCurrent != NULL) {
					/* Creation du json */
					id = str_sub(pCurrent->id, 0, 7);
					eep = str_sub(pCurrent->EEP, 0, 5);
					root = createCSON(id, eep);
					sensor = cJSON_Print(root);
					/* Ecriture des donnees */
					fprintf(f, "%s", sensor);

					cJSON_Delete(root);
					pCurrent = pCurrent->next;
					free(id);
					free(eep);
				}

			}
			/* Fermeture du fichier */
			fclose(f);
		}

		/*
		 * Enregistre dans la configuration du systeme la liste d'actionneurs
		 */
		void writeAllActuatorConfig(char* fileNameActuator, Actuator * p_actuatorList){
			Actuator * pCurrent;
			cJSON * root;
			char * actuator, *id, *eep;
			FILE *f;

			remove (fileNameActuator);
			/* Ouverture du fichier en ecriture */
			f = fopen(fileNameActuator, "w");


			if (p_actuatorList != NULL) {
				pCurrent = p_actuatorList;
				/* Parcours de la liste d'actionneurs */
				while (pCurrent != NULL) {
					/* Creation du json */
					id = str_sub(pCurrent->id, 0, 7);
					eep = str_sub(pCurrent->EEP, 0, 5);
					root = createCSON(id, eep);
					actuator = cJSON_Print(root);
					/* Ecriture des donnees */
					fprintf(f, "%s", actuator);

					cJSON_Delete(root);
					pCurrent = pCurrent->next;
					free(id);
					free(eep);
				}

			}
			/* Fermeture du fichier */
			fclose(f);
		}

		/*
		 * Enregistre dans la configuration du systeme le capteur en paramètre.
		 */
		void writeConfigSensor(char* fileNameSensor, Sensor * p_sensor) {
			cJSON * root;
			char * sensor, *id, *eep;
			FILE *f;

			/* Ouverture du fichier en ecriture */
			f = fopen(fileNameSensor, "a");

			/* Creation du json */
			id = str_sub(p_sensor->id, 0, 7);
			eep = str_sub(p_sensor->EEP, 0, 5);
			root = createCSON(id, eep);
			sensor = cJSON_Print(root);

			/* Ecriture des donnees */
			fprintf(f, "%s", sensor);

			cJSON_Delete(root);
			free(sensor);
			free(id);
			free(eep);

			/* Fermeture du fichier */
			fclose(f);
		}

		/*
		 * Ajoute dans le fichier de configuration un actionneur.
		 */
		void writeConfigActuator(char* fileNameActuator, Actuator * p_actuator){
			cJSON * root;
			char * actuator;
			FILE *f;

			/* Ouverture du fichier en ecriture */
			f = fopen(fileNameActuator, "a");

			/* Creation du json */
			root = createCSON(p_actuator->id, p_actuator->EEP);
			actuator = cJSON_Print(root);

			/* Ecriture des donnees */
			fprintf(f, "%s", actuator);

			cJSON_Delete(root);
			free(actuator);

			/* Fermeture du fichier */
			fclose(f);
		}
		