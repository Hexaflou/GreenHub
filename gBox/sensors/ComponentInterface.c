/*
 * InterfaceComposant.c
 *
 * Created on: 4 janv. 2012
 * Author: H4212
 */

/* Inclusions internes */
#include "ComponentInterface.h"
#include "Utility.h"
#include "Component.h"
#include "Test.h"
#include "EEP.h"
#include "SimulationSensor.h"

/* Inclusions externes */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Déclaration de variables */
sem_t mutex_sensorList;
Sensor* p_sensorList;
EEP* p_EEPList;

/* Fonction lançant les deux connections d'écoute avec les périphériques EnOcean et SunSpot */
int ComponentInterface(void* attr)
{
	pthread_t thread1, thread2;
	char *message1 = "Thread SunSPOT";
	char *message2 = "Thread EnOcean";
	int iret1=0, iret2=0;

	if (sem_init(&mutex_sensorList, 0, 1) == ERROR){
		perror("[ComponentInterface] Erreur dans l initialisation du semaphore pour la liste de capteurs.\n");
		return (int)ERROR;
	}

	sem_wait(&mutex_sensorList);

	p_sensorList = NULL;
	/*p_sensorList->next = NULL;*/
	p_EEPList = (EEP*)malloc(sizeof(EEP));
	p_EEPList->next = NULL;

	

	/* Initialisation des capteurs et EEP */
	initializeSensorAndEEPList(&p_sensorList, p_EEPList);

	sem_post(&mutex_sensorList);

	/* On va lancer 2 thread, un pour les SunSPOTs, un pour les capteurs EnOcean */

	/* on les créé, passe un argument on verra plus tard lequel exactement */
	/* iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);	*/
	 iret2 = pthread_create(&thread2, NULL, ListenEnOcean, (void*) message2); 

	/* on les attend
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);*/
	/*StartSimulationSensor();*/
	return (iret1 || iret2);
}


/* Fonction permettant l'écoute de périphérique SunSpot.
** Si un composant SunSpot non enregistré communique, il sera
** automatiquement enregistré dans notre liste de capteurs.
*/
void *ListenSunSpot(void *message1) {
	int sFd;
	char buffer[47], *message; /* on recevra le message en une seule fois */
    long n;
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);
    
	/* Déclaration des infos réseau */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* on assume que le serveur Java tournera sur la même machine */
	serverAddr.sin_port = htons(1337);
    
	/* On est en UDP (mode non connecté), création du socket */
	if ((sFd = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR)
	{
		perror("[ListenSunSpot] SunSPOT UDP Socket Creation Error \n");
		return (int*)ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Binding with server...\n");
    #endif
    
	/* Pas de connect à faire, il faut juste se binder */
	if (bind(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == SOCKET_ERROR)
	{
		perror("[ListenSunSpot] Socket bind Error \n");
		return (int*) SOCKET_ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Bind with socket OK\n");
    #endif
    
	while (1)
	{
        #if DEBUG > 0
            printf("[ListenSunSpot] Waiting for a message debut...\n");
        #endif
        
        /* on reçoit le message en une seule fois, comme un gros tableau de caractères */
		if ((n = recvfrom(sFd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &serverAddr, &serverAddrLen)) < 0)
		{
			perror("[ListenSunSpot] Receive Error \n");
            
			break;
        }
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Sensor message received.\n");
        #endif
        
        /*
         On va maintenant parser le message
         Il a le même format que celui des capteurs EnOcean
         */
        
        message = (char*) buffer; /* on mets ça dans un char*, passe à une chaîne de charactères */
        
        /* on regarde l'entête, vérifie que c'est bien "A55A" */
		if (strcmp(strtok(message, ";"), "A55A") != 0)
        {
            printf("[ListenSunSpot] Wrong header.\n");
        }
        
        /* on vérifie maintenant si on est bien sur un vrai capteur SunSpot : type "03" */
		if (strcmp(strtok(NULL, ";"), "03") != 0)
        {
            printf("[ListenSunSpot] Good sensor type.\n");
        }
        
        /*
         Maintenant on a, séparés par des ; :
         adresse du capteur (id)
         date/time
         luminosité
         température
         
         TODO la fin !
         */
        
        /*
         On traite l'id du capteur :
         petite adaptation (tous les SunSPOTS ont un id commençant par 0014.4F01.0000., on raccourcit)
         on regarde si les capteurs existe déjà (à chaque SunSPOT correspond une struct capteur de température, 
         et une capteur de luminosité)
         si oui, stocke ses valeurs
         si non, créé les capteurs
         */
        
        #if DEBUG > 0
            printf(strtok(NULL, ";"));
        #endif
	}
    
	free(message);
	close(sFd);
    
	return 0;
}

/* Fonction permettant l'écoute de périphériques EnOcean.
** Elle reçoit des messages du récepteur EnOcean, enlève
** l'en-tête et appelle le traitement de celui-ci.
*/
void *ListenEnOcean(void *message2)
{
	int sFd;
	char buffer[5], *message;
	long n;
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);

	/* Protocole Internet */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("134.214.105.28");
	serverAddr.sin_port = htons(5000);

	/* Creation d un socket TCP */
	if ((sFd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("[ListenEnOcean] Erreur dans la creation du socket TCP.\n");
		return (int*)ERROR;
	}

#if DEBUG > 0
	printf("[ListenEnOcean] Connection avec le serveur...\n");
#endif

	if (connect(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == -1)
	{
		perror("[ListenEnOcean] Erreur dans la connection du socket\n");
		return (int*)SOCKET_ERROR;
	}

#if DEBUG > 0
	printf("[ListenEnOcean] Connection avec le serveur OK\n");
#endif

	while (1)
	{
#if DEBUG > 0
		printf("[ListenEnOcean] Attente d un debut de message...\n");
#endif

		while (strcmp(buffer, "A55A") != 0)
		{
			if ((n = recv(sFd, buffer, sizeof(buffer - 1), 0)) < 0)
			{
				perror("[ListenEnOcean] Erreur dans la reception du message \n");
				break;
			}
			buffer[4] = '\0';
		}

#if DEBUG > 0
		printf("[ListenEnOcean] Message recu.\n");
#endif

		if ((n = recv(sFd, buffer, 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Erreur dans la reception des donnees.\n");
			break;
		}
		buffer[2] = '\0';

#if DEBUG > 0
		printf("[ListenEnOcean] Taille du buffer : %s \n", buffer);
#endif

		/* Convertit un tableau de caractere en hexadecimal en nombre decimal */
		tailleTrame = xtoi(buffer);

		/* Un octet correspond a deux caracteres */
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		/* Reception du message sans le header */
		if ((n = recv(sFd, message, tailleTrame * 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Data Reception Error \n");
			break;
		}
		message[tailleTrame * 2] = '\0';
		ManageMessage(message);
	}
	close(sFd);
	free(message);
	return 0;
}

/* Fonction traitant les messages EnOcean.
** Si le capteur emettant le message est enregistre dans notre configuration
** son message sera traite. Si le message est un message de Teach-In ce capteur pourra
** etre integre dynamiquement a notre configuration, seulement si son EEP est connu de
** l'application.
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

/* Fonction permettant de retirer la valeur d un capteur. 
** Cette valeur est retiree par le pointeur p_value, qui doit etre initialise au prealable.
** Renvoie -1 si erreur, 0 si OK.
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

int AddSensor(char id[8], char org[2], char funct[2], char type[2])
{
	return AddSensorByEEP(id, &p_sensorList, p_EEPList, org, funct, type);	
}

Sensor * getSensorList(){
	return p_sensorList;
}

sem_t getSemaphore(){
	return mutex_sensorList;
}
