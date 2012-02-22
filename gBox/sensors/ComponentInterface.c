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
#include "Component.h"
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
 * Fonction permettant l'écoute de périphérique SunSpot.
 * Si un composant SunSpot non enregistré communique, il sera
 * automatiquement enregistré dans notre liste de capteurs.
 */
void *ListenSunSpot(void *message1) {
	int sFd;
	char buffer[47], *message; /* on recevra le message en une seule fois */
    long n;
	struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);

	/* Variable pour la trame a gerer, seront utilises bien plus tard */
	char* idCapteur;
	char* dateTime;
	int temperature;
	char hexTemperature[5];
	int brightness;
	char hexBrightness[5];
	char frame[22] = {'F', 'F'}; /* toutes les autres valeurs seront des 0 */


	/* Déclaration des infos réseau */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* on assume que le serveur Java tournera sur la même machine */
	serverAddr.sin_port = htons(1337);

	/* Création du socket en UDP*/
	if ((sFd = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR)
	{
		perror("[ListenSunSpot] Erreur dans la creation du socket UDP SunSPOT ");
		return (int*)ERROR;
	}

    #if DEBUG > 0
        printf("[ListenSunSpot] Connection avec le serveur...\n");
    #endif

	/* Pas de connect à faire, il faut juste se binder */
	if (bind(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == SOCKET_ERROR)
	{
		perror("[ListenSunSpot] Erreur dans la connection avec le serveur ");
		return (int*) SOCKET_ERROR;
	}

    #if DEBUG > 0
        printf("[ListenSunSpot] Connection avec le serveur OK\n");
    #endif

	while (1)
	{
        #if DEBUG > 0
            printf("[ListenSunSpot] Attente d'un début de message...\n");
        #endif

        /* on reçoit le message en une seule fois */
		if ((n = recvfrom(sFd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &serverAddr, &serverAddrLen)) < 0)
		{
			perror("[ListenSunSpot] Erreur dans la réception ");

			break;
        }

        #if DEBUG > 0
            printf("[ListenSunSpot] Message d'un capteur reçu..\n");
        #endif

        /*
         On va maintenant parser le message
         Il a le même format que celui des capteurs EnOcean
         */

        /* message = (char*) buffer; */ /* on mets ça dans un char*, passe à une chaîne de charactères */
	/*strncpy(message, buffer, 30);

	printf(message);*/
	message = buffer;

        /* on regarde l'entête, vérifie que c'est bien "A55A" */
	if (strcmp(strtok(message, ";"), "A55A") != 0)
        {
            printf("[ListenSunSpot] Mauvais header.\n");
        }

        /* on vérifie maintenant si on est bien sur un vrai capteur SunSpot : type "03" */
	if (strcmp(strtok(NULL, ";"), "03") != 0)
        {
            printf("[ListenSunSpot] Bon type de capteur.\n");
        }

        /*
         Maintenant on a, séparés par des ; :
         adresse du capteur (id)
         date/time
         luminosité
         température

         On récupère le restant du message, pour construire une "pseudo-trame",
         similaire à ce que les capteurs EnOcean envoient
         */

        /* id du capteur :
         forme 0014.4F01.0000.5620,
         les 15 premiers charactères sont toujours les mêmes,
         du coup on raccourcit sans problèmes pour ne garder que les 4 derniers
        */

        idCapteur = str_sub(strtok(NULL, ";"), 14, 18);

        /* date et heure de la mesure : info pas utilisée pour l'instant */
        dateTime = strtok(NULL, ";");

        /* luminosité */
        brightness = atoi(strtok(NULL, ";")); /* on récupère déjà la valeur dans un int */

        if (brightness <= 0xFFFF)
        {
            sprintf(&hexBrightness[0], "%04x", brightness);
        }

        /* température - même fonctionnement */
        temperature = atoi(strtok(NULL, ";"));
        
        /* il faut qu'on applique un coefficient (diviseur)
         détail du calcul :
         (scaleMax-scaleMin)/(rangeMax/rangeMin)
         on remplace avec les valeurs, issues de la datasheet pour la scale et le range est celui d'un int :
         (165-0)/(255-0) = 0,647058824
         */
        temperature = temperature*0.647058824;

        if (temperature <= 0xFFFF)
        {
            sprintf(&hexTemperature[0], "%04x", temperature);
        }
        
        /* --- luminosité --- */
        brightness = atoi(strtok(NULL, ";")); /* on récupère déjà la valeur dans un int */
        
        /* il faut qu'on applique un coefficient de nouveau :
         la valeur envoyée par le capteur est entre 0 et 750 (cf datasheet)
         il faut que cela tienne dans un int entre 0 et 255
         
         détail du calcul :
            (750-0)/(255-0)
         */

        brightness = brightness/2.94117647;
		
        if (brightness <= 0xFFFF)
        {
            sprintf(&hexBrightness[0], "%04x", brightness);
        }
        
        /*
         On construit concrètement la pseudo trame maintenant :
            FF : code org
            00 : "trou"
            LL : valeur de la luminosité
            TT : valeur de la température
            00 : "trou"
            0000 : "faux" ID
            IDID : ID réel du SPOT
            0000 : "trou"
        */

        memcpy(&frame[4], hexBrightness, 2); /* recopie 2 octets */
        memcpy(&frame[6], hexTemperature, 2);

        memcpy(&frame[14], idCapteur, 4); /* finalement, l'id */
        
        #if DEBUG > 0
            printf(frame);
        #endif
        
        ManageMessage(frame);
	}
    
	free(message);
	close(sFd);
    
	return 0;
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

/* 
 * Permet d ajouter un capteur a partir de son EEP et son id 
 * Sortie : Renvoie 0 si tout s'est bien passe, -1 si l EEP n est pas supporte et -2 si l EEP est inconnu.
 */
int AddSensor(char id[8], char org[2], char funct[2], char type[2])
{
	return AddComponentByEEP(id, (void**)&p_sensorList, p_EEPList, org, funct, type);	
}

/* 
 * Fonction permettant de retirer l etat d un actionneur. 
 * Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
 * Renvoie -1 si erreur, 0 si OK.
 */
int GetStatusFromSensor(char id[10], float * p_value){
	char realId[8];
	Actuator* p_currentActuator;
	strcpy(realId,str_sub(id, 0, 7));	/* L'ID reel d un peripherique est compose de 8 caracteres */

	*p_value = 0;

	sem_wait(&mutex_actuatorList);
	p_currentActuator = p_actuatorList;

	/* Recherche de l'actionneur dans la liste d'actionneurs */
	while (p_currentActuator != NULL)
	{
		if (strcmp(p_currentActuator->id, realId) == 0)
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
 * Renvoie la liste de capteurs. 
 */
Sensor * getSensorList(){
	return p_sensorList;
}

/* 
 *Renvoie le mutex protegeant la liste de capteurs 
 */
sem_t getSemaphore(){
	return mutex_sensorList;
}

/* 
 * Agit sur un actionneur de notre liste d actionneurs.
 * Entrees :
 * 	id : id de notre actionneur.
 * 	value : la valeur a donne dans l action
 */
int ActionActuator(char id[13], float value){
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
