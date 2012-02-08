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
#include <semaphore.h>

sem_t mutex_sensorList;
Sensor* p_sensorList;
EEP* p_EEPList;

int ComponentInterface()
{
	pthread_t thread1, thread2;
	char *message1 = "Thread SunSPOT";
	char *message2 = "Thread EnOcean";
	int iret1, iret2;
	void* ptt;	


	if (sem_init(&mutex_sensorList, 0, 1) == ERROR){
		perror("[ComponentInterface] sensorList's semaphor's initialization failed.\n");
		return (int)ERROR;
	}

	sem_wait(&mutex_sensorList);

	p_sensorList = (Sensor*)malloc(sizeof(Sensor*));
	p_sensorList->next = NULL;
	p_EEPList = NULL;

	

	/* Initialisation des capteurs et EEP */
	initializeSensorAndEEPList(p_sensorList, p_EEPList);

	sem_post(&mutex_sensorList);

	/* On va lancer 2 thread, un pour les SunSPOTs, un pour les capteurs EnOcean */

	/* on les créé, passe un argument on verra plus tard lequel exactement */
	/* iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);	*/
	 iret2 = pthread_create(&thread2, NULL, ListenEnOcean, (void*) message2); 

	gLogsInit();
	/*ListenEnOcean(ptt); */

	/* on les attend
	pthread_join(thread1, NULL);	*/
	pthread_join(thread2, NULL);	
	gLogsClose();
	return 0;
}

void *ListenSunSpot(void *message1) {
	int sFd;
	char buffer[46], *message; /* on recevra le message en une seule fois */
    long n;
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);
    
	/* Internet Protocol */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* on assume que le serveur Java sera local */
	serverAddr.sin_port = htons(1337);

	/* UDP, not TCP, Socket Creation */
	if ((sFd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("[ListenSunSpot] SunSPOT UDP Socket Creation Error \n");
		return (int*)ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Binding with server...\n");
    #endif

	/* on est en UDP, pas de connect à faire, mais un bind */
	if (bind(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == -1)
	{
		perror("[ListenSunSpot] Socket bond Error \n");
		return (int*)SOCKET_ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Bind with server OK\n");
    #endif

	while (1)
	{
    #if DEBUG > 0
            printf("[ListenSunSpot] Waiting for a message debut...\n");
    #endif

		while (strcmp(buffer, "A55A") != 0)
		{
			if ((n = recv(sFd, buffer, sizeof(buffer - 1), 0)) < 0)
			{
				perror("[ListenSunSpot] Receive Error \n");
				break;
			}
			buffer[4] = '\0';
		}
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Message received.\n");
        #endif

		if ((n = recv(sFd, buffer, 2, 0)) < 0)
		{
			perror("[ListenSunSpot] Data Reception Error \n");
			break;
		}
		buffer[2] = '\0';
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Buffer length : %s \n", buffer);
        #endif

		/* Convert array of char corresponding to hexadecimal number to int */
		tailleTrame = xtoi(buffer);

		/* One byte corresponds to 2 char */
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		/* Reception of the message without the header */
		if ((n = recv(sFd, message, tailleTrame * 2, 0)) < 0)
		{
			perror("[ListenSunSpot] Data Reception Error \n");
			break;
		}

		message[tailleTrame * 2] = '\0';
		ManageMessage(message);
	}

	close(sFd);

	return 0;
}

void *ListenEnOcean(void *message2)
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
		ManageMessage(message);
	}
	close(sFd);
	return 0;
}

void ManageMessage(char* message)
{
	int org, teachIn;
	Sensor* currentSensor;


#if DEBUG > 0
	printf("Message : %s \n", message);
#endif

	sem_wait(&mutex_sensorList);
	currentSensor = p_sensorList;

	/* Search of the sensor in the sensors' list */
	while (currentSensor != NULL)
	{
		if (strcmp(str_sub(currentSensor->id, 0, 7), str_sub(message, 10, 17)) == 0)
		{
			/*printf("Détecteur présent dans la liste ! \n");*/
			currentSensor->decodeMessage(message, *currentSensor);			
			break;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}
	sem_post(&mutex_sensorList);
	if (currentSensor != NULL){
		GetInfoFromSensor(currentSensor->id);
	}

	/* If the sensor isn't in the sensors' list */

}

float GetInfoFromSensor(char id[10]){
	char realId[8];
	Sensor* currentSensor;
	strcpy(realId,str_sub(id, 0, 7));

	sem_wait(&mutex_sensorList);
	currentSensor = p_sensorList;
	/* Search of the sensor in the sensors' list */
	while (currentSensor != NULL)
	{
		if (strcmp(currentSensor->id, realId) == 0)
		{
			/*printf("Détecteur présent dans la liste ! \n");*/
			sem_post(&mutex_sensorList);
			return currentSensor->value;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}
	sem_post(&mutex_sensorList);
	return (float)0;
}

int AddSensor(char id[8], char org[2], char funct[2], char type[2])
{
	return 0;
}

