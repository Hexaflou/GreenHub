/*
 * InterfaceComposant.c
 *
 * Interface avec les peripheriques (capteurs/actionneurs) permettant au serveur de communiquer avec ceux-ci.
 * 
 * Author: H4212
 */


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

/* Déclaration de variables */
static mqd_t smq;
static sem_t mutex_sensorList;
static sem_t mutex_actuatorList;
static Sensor* p_sensorList;
static Actuator* p_actuatorList;
static EEP* p_EEPList;

/* Pour le mode simulation */
static pthread_t threadReceptor;
static mqd_t mqReceptor;

/* Fonction lançant les deux connections d'écoute avec les périphériques EnOcean et SunSpot */
int ComponentInterface(void* attr)
{
	pthread_t thread1;
	char *message1 = "Thread SunSPOT";	
	int iret1=0;
	/* Création des mutex pour la liste de capteurs et la liste d'actionneurs */
	if (sem_init(&mutex_sensorList, 0, 1) == ERROR){
		perror("[ComponentInterface] Erreur dans l initialisation du semaphore pour la liste de capteurs.\n");
		return (int)ERROR;
	}
	if (sem_init(&mutex_actuatorList, 0, 1) == ERROR){
		perror("[ComponentInterface] Erreur dans l initialisation du semaphore pour la liste d actionneurs.\n");
		return (int)ERROR;
	}

	/* Initialisation des listes de capteurs et d'actionneurs */
	sem_wait(&mutex_sensorList);
	p_sensorList = NULL;
	p_actuatorList = NULL;	
	p_EEPList = (EEP*)malloc(sizeof(EEP));
	p_EEPList->next = NULL;
	
	/* Chargement des capteurs et EEP */
	readConfig("sensors.txt", "eep.txt", "actuators.txt", &p_sensorList, &p_actuatorList, p_EEPList);
	sem_post(&mutex_sensorList);

	/* Mode Simulation (récepteur EnOcean, capteurs et actionneurs) */
	{		
		mqReceptor = comSimulationReceptorTaskInit();
		pthread_create(&threadReceptor, NULL, StartSimulationSensor, (void*) &mqReceptor);		
	}
	
	/* Création et lancement des deux tâches permettant de communiquer avec le récepteur EnOcean */
	smq = comReceptorTaskInit();
	
	/* Lancement de 2 threads pour SunSPOTs et pour EnOcean */
	 /*iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);*/


	return (iret1);
}


/* 
 * Fonction traitant les messages EnOcean.
 * Si le capteur emettant le message est enregistre dans notre configuration
 * son message sera traite. Si le message est un message de Teach-In ce capteur pourra
 * etre integre dynamiquement a notre configuration, seulement si son EEP est connu de
 * l'application.
 */
void ManageMessage(char* message)
{
	Sensor* currentSensor;
	#if DEBUG > 0
		printf("Message : %s \n", message);
	#endif

	sem_wait(&mutex_sensorList);
	currentSensor = p_sensorList;

	/* Recherche du capteur dans la liste de capteurs */
	while (currentSensor != NULL)
	{
		if (strcmp(str_sub(currentSensor->id, 0, 7), str_sub(message, 10, 17)) == 0) /* Détecteur présent dans la liste */
		{
			/*printf("Détecteur présent dans la liste ! \n");*/
			currentSensor->decodeMessage(message, currentSensor);
			currentSensor = currentSensor->next;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}
	sem_post(&mutex_sensorList);	

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
int AddComponent(char* id, char org[2], char funct[2], char type[2])
{
	int result;
	if (strlen(id) > 8)
	{
		Sensor* currentSensor;
		sem_wait(&mutex_sensorList);
		currentSensor = p_sensorList;
		while (currentSensor != NULL){
			if (strcmp(currentSensor->id, id) == 0)
			{
				sem_post(&mutex_sensorList);
				return COMPONENT_ALREADY_EXIST;
			}
			currentSensor = currentSensor->next;
		}
		result = AddComponentByEEP(id, (void**)&p_sensorList, p_EEPList, org, funct, type);
		sem_post(&mutex_sensorList);
	}
	else
	{
		Actuator* currentActuator;
		sem_wait(&mutex_actuatorList);
		currentActuator = p_actuatorList;
		while (currentActuator != NULL){
			if (strcmp(currentActuator->id, id) == 0)
			{
				sem_post(&mutex_actuatorList);
				return COMPONENT_ALREADY_EXIST;
			}
			currentActuator = currentActuator->next;
		}
		result = AddComponentByEEP(id, (void**)&p_sensorList, p_EEPList, org, funct, type);
		sem_post(&mutex_actuatorList);
	}
	return result;
}


/*************** CAPTEUR ***************/

/*
 * Renvoie la liste de capteurs.
 */
Sensor * getSensorList(){
	return p_sensorList;
}

/*
 *Renvoie le mutex protegeant la liste de capteurs
 */
sem_t getSemSensor(){
	return mutex_sensorList;
}

/* 
 * Fonction permettant de retirer la valeur d un capteur. 
 * Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
 * Renvoie -1 si erreur, 0 si OK.
*/
int GetInfoFromSensor(char id[10], float * p_value){
	char realId[8];
	Sensor* currentSensor;
	strcpy(realId,str_sub(id, 0, 7));	/* L'ID reel d un peripherique est compose de 8 caracteres */
	*p_value = 0;

	sem_wait(&mutex_sensorList);
	currentSensor = p_sensorList;

	/* Recherche du capteur dans la liste de capteurs */
	while (currentSensor != NULL)
	{
		if (strcmp(currentSensor->id, realId) == 0)
		{
			/*printf("Détecteur présent dans la liste ! \n");*/
			sem_post(&mutex_sensorList);
			*p_value = currentSensor->value;
			return OK;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}
	sem_post(&mutex_sensorList);	
	return ERROR;
}


/*************** ACTIONNEUR ***************/

/*
 * Renvoie la liste d'actionneurs.
 */
Actuator * getActuatorList(){
	return p_actuatorList;
}

/*
 *Renvoie le mutex protegeant la liste de capteurs
 */
sem_t getSemActuator(){
	return mutex_actuatorList;
}

/* 
 * Fonction permettant de retirer l etat d un actionneur. 
 * Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
 * Renvoie -1 si erreur, 0 si OK.
 */
int GetStatusFromActuator(char* id, float * p_value){
	Actuator* p_currentActuator;
	if (strlen(id)!=10){
		return INVALID_ID;
	}	

	*p_value = 0;

	sem_wait(&mutex_actuatorList);
	p_currentActuator = p_actuatorList;

	/* Recherche de l'actionneur dans la liste d'actionneurs */
	while (p_currentActuator != NULL)
	{
		if (strcmp(p_currentActuator->id, id) == 0)
		{
			/*printf("Actionneur présent dans la liste ! \n");*/
			sem_post(&mutex_actuatorList);
			*p_value = p_currentActuator->status;
			return OK;
		}
		else
		{
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
int ActionActuator(char* id, float value){
	Actuator* p_currentActuator;
	sem_wait(&mutex_actuatorList);
	p_currentActuator = p_actuatorList;

	/* Recherche de l'actionneur dans la liste d'actionneurs */
	while (p_currentActuator != NULL)
	{
		if (strcmp(p_currentActuator->id, id) == 0)
		{
			/*printf("Actionneur présent dans la liste ! \n");*/
			sem_post(&mutex_actuatorList);
			p_currentActuator->action(value, p_currentActuator,smq);
			return OK;
		}
		else
		{
			p_currentActuator = p_currentActuator->next;
		}
	}
	sem_post(&mutex_actuatorList);	
	return ERROR;
}
