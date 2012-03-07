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
#include "../../libs/gMemory/gMemory.h"

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
#define CONFIG_CJSON_LENGTH 400
#define ID_ACTUATOR_LENGTH 11

/***************************PRIVEE***********************/
/* Déclaration de variables */
static struct SmqReturn smqReturn;
static sem_t mutex_sensorList;
static sem_t mutex_actuatorList;
static Sensor* p_sensorList;
static Actuator* p_actuatorList;
static EEP* p_EEPList;
static int alreadyInitialized = 0;
static char * sensorConfigTxt = NULL;
static char * actuatorConfigTxt = NULL;
static char * eepConfigTxt = NULL;
static char * receptorIPTxt = NULL;
static char * receptorIDTxt = NULL;

/* Pour le mode simulation */
/* static mqd_t mqReceptor; */

/************************PUBLIC***************************************/

/* Fonction lançant les deux connections d'écoute avec les périphériques EnOcean et SunSpot */
int ComponentInterfaceInit() {
	/* Objet cJSON permettant de récupérer les informations du fichier de configuration CONFIG_FILE */
	cJSON * root, * receptorIP, * receptorPort, * receptorID, *eepConfig, *sensorConfig, *actuatorConfig;
	char c;
	char txt[CONFIG_CJSON_LENGTH];
	int nbOpenedAccolade, receptorPortInt;
	FILE *f;

	if (alreadyInitialized) {
		return ERROR;
	}

	/* Ouverture du fichier en lecture */
	f = fopen("configuration.json", "r");

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
				printf("[ComponentInterface] IP du récepteur invalide dans le fichier configuration.txt\n");
				return ERROR;
			}
			receptorIPTxt = (char*)gmalloc(sizeof(char)*strlen(receptorIP->valuestring)+1);
			strcpy(receptorIPTxt,receptorIP->valuestring);

			/* Receptor Port */
			receptorPort = cJSON_GetObjectItem(root, "receptorPort");
			if (receptorPort == NULL) {
				printf("[ComponentInterface] Port du récepteur incorrect dans le fichier configuration.txt\n");
				return ERROR;
			}
			receptorPortInt = receptorPort->valueint;

			/* Receptor ID */
			receptorID = cJSON_GetObjectItem(root, "receptorID");
			if (receptorID == NULL) {
				printf("[ComponentInterface] ID du récepteur invalide dans le fichier configuration.txt\n");
				return ERROR;
			}
			receptorIDTxt  = (char*)gmalloc(sizeof(char)*strlen(receptorID->valuestring)+1);
			strcpy(receptorIDTxt ,receptorID->valuestring);

			/* Fichier de config. EEP */
			eepConfig = cJSON_GetObjectItem(root, "eepConfig");
			if (eepConfig == NULL) {
				printf("[ComponentInterface] Fichier de configuration EEP incorrect dans le fichier configuration.txt\n");
				return ERROR;
			}
			eepConfigTxt  = (char*)gmalloc(sizeof(char)*strlen(eepConfig->valuestring)+1);
			strcpy(eepConfigTxt,eepConfig->valuestring);

			/* Fichier de config. Capteur */
			sensorConfig = cJSON_GetObjectItem(root, "sensorConfig");
			if (sensorConfig == NULL) {
				printf("[ComponentInterface] Fichier de configuration Sensor incorrect dans le fichier configuration.txt\n");
				return ERROR;
			}
			sensorConfigTxt = (char*)gmalloc(sizeof(char)*strlen(sensorConfig->valuestring)+1);
			strcpy(sensorConfigTxt,sensorConfig->valuestring);

			/* Fichier de config. Actionneur */
			actuatorConfig = cJSON_GetObjectItem(root, "actuatorConfig");
			if (actuatorConfig == NULL) {
				printf("[ComponentInterface] Fichier de configuration Actuator incorrect dans le fichier configuration.txt\n");
				return ERROR;
			}
			actuatorConfigTxt = (char*)gmalloc(sizeof(char)*strlen(actuatorConfig->valuestring)+1);
			strcpy(actuatorConfigTxt,actuatorConfig->valuestring);
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

	ActuatorInterfaceInit(receptorIDTxt);

	/* Initialisation des listes de capteurs et d'actionneurs */
	sem_wait(&mutex_sensorList);
	p_sensorList = NULL;
	p_actuatorList = NULL;
	p_EEPList = (EEP*) gmalloc(sizeof (EEP));
	p_EEPList->next = NULL;

	/* Chargement des capteurs et EEP */
	if (readConfig(sensorConfigTxt, eepConfigTxt, actuatorConfigTxt, &p_sensorList, &p_actuatorList, p_EEPList) == ERROR)
		return ERROR;
	sem_post(&mutex_sensorList);

	/* Mode Simulation (récepteur EnOcean, capteurs et actionneurs) */
	/*
	 *	{
	 *	mqReceptor = comSimulationReceptorTaskInit();
	 *	StartSimulationSensor(mqReceptor);
		}*/

	/* Création et lancement des deux tâches permettant de communiquer avec le récepteur EnOcean */
	smqReturn = comReceptorTaskInit(receptorIPTxt, receptorPortInt);

	/* Création et lancement de la tâche permettant de dialoguer avec les capteurs SunSpot */
	comSunSpotTaskInit();

	/* Mode Hors Simulation*/
	StartSimulationSensor(smqReturn.smqSimulation);

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
	/*comSimulationReceptorTaskClose();*/
	StopSimulationSensor();
	destroyEEPList(p_EEPList); /* Désalloue p_EEPList */
	destroyComponentList(p_sensorList, p_actuatorList); /* Désalloue p_EEPList */
	if (sensorConfigTxt != NULL){
		gfree(sensorConfigTxt);
	}
	if (actuatorConfigTxt != NULL){
		gfree(actuatorConfigTxt);
	}
	if (eepConfigTxt != NULL){
		gfree(eepConfigTxt);
	}
	if (receptorIPTxt != NULL){
		gfree(receptorIPTxt);
	}
	if (receptorIDTxt != NULL){
		gfree(receptorIDTxt);
	}
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
	char *org;
	char *byte_0;
	byte_0 = str_sub(message,8,9);
	messageId = str_sub(message, 10, 17);
	#if DEBUG > 0
	printf("[ComponentInterface] Message : %s \n", message);
	#endif

	org = str_sub(message,0,1);

	if ( (strcmp(org,"07") == 0) && (!(xtoi(byte_0) & 8)) ){	/* Si le capteur est de type 4BS et le message est en mode Teach-In */		
		if (xtoi(byte_0) & 128) /* Si les valeurs ORG-FUNCT-TYPE sont récupérables */{
			#if DEBUG > 0
			printf("[ComponentInterface] Message en mode Teach-In.\n");
			#endif
			char funct[3], type[3];
			char * byte_3, *byte_2;
			int functInt, typeInt;
			byte_3 = str_sub(message, 2,3);
			byte_2 = str_sub(message, 4,5);
			functInt = (xtoi(byte_3) & 252)>>2;
			typeInt = ( ((xtoi(byte_3) & 3) << 5) + ((xtoi(byte_2) & 248)>>3));
			sprintf(funct,"%02X", functInt);
			sprintf(type,"%02X", typeInt);
			gfree(byte_3);
			gfree(byte_2);
			if (AddComponent(messageId, org, funct, type) == OK){
				/* Construction d'un message JSON à envoyer au serveur Web */
				char * msgJSON_Parse, eep[7];
				cJSON * data = cJSON_CreateObject();
				
				strcpy(eep,org);
				strcat(eep,funct);
				strcat(eep,type);

				printf("[ComponentInterface] Ajout d'un composant par Learn. ID : %s; EEP : %s\n",messageId, eep);
				
				cJSON_AddStringToObject(data, "msg_type", "new_sensor");
				cJSON_AddStringToObject(data, "hardware_id", messageId);
				cJSON_AddStringToObject(data, "eep", eep);
    				msgJSON_Parse = cJSON_Print(data);

    				gCommunicationSend(msgJSON_Parse);
				gfree(msgJSON_Parse);
				cJSON_Delete(data);
			}				
			gfree(messageId);
			exit(0);
		}
	}
	sem_wait(&mutex_sensorList);
	currentSensor = p_sensorList;
	sensorRealId = str_sub(currentSensor->id, 0, 7);
	/* Recherche du capteur dans la liste de capteurs */
	while (currentSensor != NULL) {
		if (strcmp(sensorRealId, messageId) == 0) /* Détecteur présent dans la liste */ {
			/*printf("Détecteur présent dans la liste ! ID : %s \n", sensorRealId);*/
			currentSensor->decodeMessage(message, currentSensor);
		}
		gfree(sensorRealId);
		currentSensor = currentSensor->next;
		sensorRealId = str_sub(currentSensor->id, 0, 7);
	}
	if (sensorRealId != NULL)
		gfree(sensorRealId);
	sem_post(&mutex_sensorList);
	gfree(messageId);
	/* If the sensor isn't in the sensors' list */

}

/*****************************************************
 *
 *		API CAPTEUR/ACTIONNEUR
 *
 ****************************************************/

/*
 * Permet d ajouter un composant a partir de son EEP et son id
 * Sortie : Renvoie 0 si tout s'est bien passe, -1 si l EEP n est pas supporte,-2 si l EEP est inconnu et -3 si l'ID est déjà présent.
 */
int AddComponent(char* id, char org[2], char funct[2], char type[2]) {
	int result;
	char * hardware_id;
	if (strlen(id) < ID_ACTUATOR_LENGTH) {	/* Si le composant est un capteur */
		Sensor* currentSensor;
		sem_wait(&mutex_sensorList);
		currentSensor = p_sensorList;
		hardware_id = str_sub(currentSensor->id,0,7);

		/* On recherche si le capteur n'existe pas déjà */
		while (currentSensor != NULL && currentSensor->next != NULL) {
			if (strcmp(hardware_id, id) == 0) {
				sem_post(&mutex_sensorList);
				gfree(hardware_id);
				return COMPONENT_ALREADY_EXIST;
			}
			currentSensor = currentSensor->next;
			gfree(hardware_id);
			hardware_id = str_sub(currentSensor->id,0,7);
		}
		result = AddComponentByEEP(id, (void**) & p_sensorList, p_EEPList, org, funct, type);

		/* Recherche dans la liste de capteurs du capteur que l'on vient d'ajouter */
		while (currentSensor != NULL) {
			if (strcmp(hardware_id, id) == 0) {
				writeConfigSensor(sensorConfigTxt, currentSensor);	/* Ajout dans le fichier de configuration du capteur */
				gfree(hardware_id);
				break;
			}
			currentSensor = currentSensor->next;
			gfree(hardware_id);
			if (currentSensor != NULL)
				hardware_id = str_sub(currentSensor->id,0,7);
		}
		sem_post(&mutex_sensorList);
	} else {
		Actuator* currentActuator;
		char * arg_hardware_id; /* ID réel de l'actionneur sur 2 caractères */
		sem_wait(&mutex_actuatorList);
		currentActuator = p_actuatorList;

		/* On retire les deux derniers caractères de l'id de l'actionneur dans la liste, qui correspondent à son ID réel */
		hardware_id = str_sub(currentActuator->id,strlen(currentActuator->id)-2, strlen(currentActuator->id)-1);
		arg_hardware_id = str_sub(id,strlen(currentActuator->id)-2, strlen(currentActuator->id)-1);

		/* On vérifie si l'actionneur existe déjà */
		while (currentActuator != NULL && currentActuator->next != NULL) {
			if (strcmp(hardware_id, arg_hardware_id) == 0) {
				sem_post(&mutex_actuatorList);
				gfree(hardware_id);
				gfree(arg_hardware_id);
				return COMPONENT_ALREADY_EXIST;
			}
			currentActuator = currentActuator->next;
			gfree(hardware_id);
			hardware_id = str_sub(currentActuator->id,strlen(currentActuator->id)-2, strlen(currentActuator->id)-1);
		}
		result = AddComponentByEEP(id, (void**) & p_actuatorList, p_EEPList, org, funct, type);

		/* Recherche dans la liste de capteurs du capteur que l'on vient d'ajouter */
		while (currentActuator != NULL) {
			if (strcmp(hardware_id, arg_hardware_id) == 0) {
				writeConfigActuator(actuatorConfigTxt, currentActuator);	/* Ajout dans le fichier de configuration du capteur */
				gfree(hardware_id);
				break;
			}
			currentActuator = currentActuator->next;
			gfree(hardware_id);
			if (currentActuator != NULL)
				hardware_id = str_sub(currentActuator->id,strlen(currentActuator->id)-2, strlen(currentActuator->id)-1);
		}
		gfree(arg_hardware_id);
		sem_post(&mutex_actuatorList);
	}
	return result;
}

/*
 * Fonction permettant d’ajouter un composant.
 */
int RemoveComponent(char * id){
	char * hardware_id;
	if (strlen(id) < ID_ACTUATOR_LENGTH) {	/* Si le composant est un capteur */
		Sensor* currentSensor, *precedentSensor;
		sem_wait(&mutex_sensorList);
		precedentSensor = p_sensorList;
		if (precedentSensor != NULL)
			currentSensor = precedentSensor->next;
		else	/* S'il n'y a qu'un seul capteur dans la liste */
			currentSensor = precedentSensor;
		hardware_id = str_sub(currentSensor->id,0,7);

		/* On recherche si le capteur n'existe pas déjà */
		while (precedentSensor != NULL && currentSensor != NULL) {
			if (strcmp(hardware_id, id) == 0) {
				precedentSensor->next = currentSensor->next;

				if (currentSensor->rangeData != NULL)
					gfree(currentSensor->rangeData);
				if (currentSensor == precedentSensor) /* S'il ne reste qu'un seul capteur dans la liste */
				{
					gfree(currentSensor);
					gfree(hardware_id);
					break;
				}
			}else{
				precedentSensor = currentSensor;
			}
			currentSensor = precedentSensor->next;
			gfree(hardware_id);
			hardware_id = str_sub(currentSensor->id,0,7);
		}
		writeAllSensorConfig(sensorConfigTxt, p_sensorList);
		sem_post(&mutex_sensorList);
	} else {
		Actuator* currentActuator, *precedentActuator;
		sem_wait(&mutex_actuatorList);
		precedentActuator = p_actuatorList;
		if (precedentActuator != NULL)
			currentActuator = precedentActuator->next;
		else	/* S'il n'y a qu'un seul capteur dans la liste */
			currentActuator = precedentActuator;

		/* On vérifie si l'actionneur existe déjà */
		while (precedentActuator != NULL && currentActuator != NULL) {
			if (strcmp(currentActuator->id, id) == 0) {
				precedentActuator->next = currentActuator->next;

				if (currentActuator->rangeData != NULL)
					gfree(currentActuator->rangeData);
				if (currentActuator == precedentActuator) /* S'il ne reste qu'un seul capteur dans la liste */
				{
					gfree(currentActuator);
					break;
				}
			}else{
				precedentActuator = currentActuator;
			}
			currentActuator = precedentActuator->next;
		}
		writeAllActuatorConfig(actuatorConfigTxt, p_actuatorList);
		sem_post(&mutex_actuatorList);
	}
	return OK;
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
	gfree(realId);
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
			p_currentActuator->action(value, p_currentActuator, smqReturn.smq);
			return OK;
		} else {
			p_currentActuator = p_currentActuator->next;
		}
	}
	sem_post(&mutex_actuatorList);
	return ERROR;
}
