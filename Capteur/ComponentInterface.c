/*
 * InterfaceComposant.c
 *
 * Created on: 4 janv. 2012
 * Author: H4212
 */

/* Internal Includes */
#include "ComponentInterface.h"
#include "memoryGHome.h"
#include "Utility.h"
#include "Component.h"
#include "Test.h"
#include "gLogs.h"

/* External Includes */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

int ComponentInterface()
{
	pthread_t thread1, thread2;
	char *message1 = "Thread SunSPOT";
	char *message2 = "Thread EnOcean";
	int iret1, iret2;
	void* ptt;
	Sensor** pp_sensorList;
	EEP* p_EEPList;

	pp_sensorList = (Sensor**)malloc(sizeof(Sensor*));
	p_EEPList = NULL;


	/* Initialisation des capteurs et EEP */
	initializeSensorAndEEPList(pp_sensorList, p_EEPList);


	/* On va lancer 2 thread, un pour les SunSPOTs, un pour les capteurs EnOcean */

	/* on les créé, passe un argument on verra plus tard lequel exactement */
	iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);
	//iret2 = pthread_create(&thread2, NULL, ListenEnOcean, (void*) message2); 

	gLogsInit();

	/*ListenEnOcean(ptt); */

	/* on les attend
	pthread_join(thread1, NULL);	*/
	pthread_join(thread2, NULL);	
	gLogsClose();

	return 0;
}

/*
 Fonction qui va écouter (dans un thread) sur un port spécifique
 les messages renvoyés par le serveur des SunSpots
*/
void *ListenSunSpot(void *ptr) {
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

	close(sFd);

	return 0;
}

void *ListenEnOcean(void *p_sensorList)
{
	int sFd;
	char buffer[5], *message;
	long n;
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);

	/* Internet Protocol */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("134.214.105.28");
	serverAddr.sin_port = htons(5000);

	/* TCP Socket Creation */
	if ((sFd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("[ListenEnOcean] TCP Socket Creation Error \n");
		return (int*)ERROR;
	}

    #if DEBUG > 0
        printf("[ListenEnOcean] Connection with the server...\n");
    #endif

	if (connect(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == -1)
	{
		perror("[ListenEnOcean] Socket connection Error \n");
		return (int*)SOCKET_ERROR;
	}

    #if DEBUG > 0
        printf("[ListenEnOcean] Connection with the server OK\n");
    #endif

	while (1)
	{
        #if DEBUG > 0
            printf("[ListenEnOcean] Waiting for a message debut...\n");
        #endif

		while (strcmp(buffer, "A55A") != 0)
		{
			if ((n = recv(sFd, buffer, sizeof(buffer - 1), 0)) < 0)
			{
				perror("[ListenEnOcean] Receive Error \n");
				break;
			}
			buffer[4] = '\0';
		}

		#if DEBUG > 0
			printf("[ListenEnOcean] Message received.\n");
		#endif

		if ((n = recv(sFd, buffer, 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Data Reception Error \n");
			break;
		}
		buffer[2] = '\0';

		#if DEBUG > 0
			printf("[ListenEnOcean] Buffer length : %s \n", buffer);
		#endif

		/* Convert array of char corresponding to hexadecimal number to int */
		tailleTrame = xtoi(buffer);

		/* One byte corresponds to 2 char */
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		/* Reception of the message without the header */
		if ((n = recv(sFd, message, tailleTrame * 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Data Reception Error \n");
			break;
		}
		message[tailleTrame * 2] = '\0';
		ManageMessage(message, (Sensor * ) p_sensorList);
	}
	close(sFd);
	return 0;
}

void ManageMessage(char* message, Sensor * p_sensorList)
{
	int org, teachIn;
	Sensor* currentSensor;


	currentSensor = p_sensorList;

	#if DEBUG > 0
		printf("Message : %s \n", message);
	#endif

	/*org = atoi(str_sub(message, 0, 1));*/ /* Extract from the message the type of sensor */
	/*teachIn = atoi(str_sub(message, 8, 9));*/ /* Extract from the message the characters indicating the teach-in message */

	/* Search of the sensor in the sensors' list */
	while (currentSensor != NULL)
	{
		if (strcmp(str_sub(currentSensor->id, 0, 7), str_sub(message, 10, 17)) == 0)
		{
		/*	printf("Détecteur présent dans la liste ! \n");*/
			currentSensor->decodeMessage(message, *currentSensor);
			break;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}

	/* If the sensor isn't in the sensors' list */

}

float GetInfoFromSensor(char message[10], Sensor * p_sensorList){
	char id[8];
	Sensor* currentSensor;

	currentSensor = p_sensorList;

	strcpy(id,str_sub(message, 0, 7));

	/* Search of the sensor in the sensors' list */
	while (currentSensor != NULL)
	{
		if (strcmp(currentSensor->id, id) == 0)
		{
			/*printf("Détecteur présent dans la liste ! \n");*/
			return currentSensor->value;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}

	return (float)0;
}

int AddSensor(char id[8], char org[2], char funct[2], char type[2])
{
	return 0;
}

