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

<<<<<<< HEAD
	/* On va lancer 2 thread, un pour les SunSPOTs, un pour les capteurs EnOcean */

	// on les créé, passe un argument on verra plus tard lequel exactement
	// iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);
	//iret2 = pthread_create(&thread2, NULL, ListenEnOcean, (void*) message2);

	ListenEnOcean(ptt);

	// on les attend
	//pthread_join(thread1, NULL);
	//pthread_join(thread2, NULL);

	return 0;
}

void *ListenSunSpot(void *ptr)
{
	int sFd;
	char buffer[5], *message;
	long n;
=======
int ComponentInterface() {
    pthread_t thread1, thread2;
    char *message1 = "Thread SunSPOT";
    char *message2 = "Thread EnOcean";
    int iret1, iret2;
    
    /* On va lancer 2 thread, un pour les SunSPOTs, un pour les capteurs EnOcean */
    
    // on les créé, passe un argument on verra plus tard lequel exactement
    iret1 = pthread_create(&thread1, NULL, ListenSunSpot, (void*) message1);
    //iret2 = pthread_create(&thread2, NULL, ListenEnOcean, (void*) message2);
    
    // on les attend
    pthread_join(thread1, NULL);
    //pthread_join(thread2, NULL);
    
    return 0;
}

void *ListenSunSpot(void *ptr) {
	int sFd;
	char buffer[46], *message; // on recevra le message en une seule fois
    long n;
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);
    
	/* Internet Protocol */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY; // écoute sur toutes les IPs locales
	serverAddr.sin_port = htons(1337);
    
	/* UDP, not TCP, Socket Creation */
	if ((sFd = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR)
	{
		perror("[ListenSunSpot] SunSPOT UDP Socket Creation Error \n");
		return ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Binding with server...\n");
    #endif
    
    // on est en UDP, pas de connect à faire, mais un bind
	if (bind(sFd, (struct sockaddr*) &serverAddr, serverAddrLen) == -1)
    {
		perror("[ListenSunSpot] Socket bind Error \n");
		return SOCKET_ERROR;
	}
    
    #if DEBUG > 0
        printf("[ListenSunSpot] Bind with server OK\n");
    #endif
    
	while (1)
    {
        #if DEBUG > 0
            printf("[ListenSunSpot] Waiting for a message debut...\n");
        #endif
        
        // fonctionnement un petit peu différent : on reçoit en une seule fois
        n = recvfrom(sFd, buffer, sizeof(buffer), 0, (struct sockaddr*) &serverAddr, &serverAddrLen);
        
        if (n < 0)
        {
            perror("[ListenSunSpot] Receive Error \n");
            break;
        }
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Message received : %s \n", buffer);
        #endif
        
        // vérifie si le header est bon
        if (strncmp(buffer, "A55Axx", 4) != 0)
        {
			printf("[ListenSunSpot] Invalid message received \n");
			break;            
        }
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Correct message received.\n");
        #endif
        
		/*while (strcmp(buffer, "A55A") != 0)
        {
			if ((n = recvfrom(sFd, buffer, sizeof (buffer - 1), 0, (struct sockaddr*) &serverAddr, &serverAddrLen)) < 0)
			{
				perror("[ListenSunSpot] Receive Error \n");
				break;
			}
			buffer[4] = '\0';
		}
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Message received.\n");
        #endif
        
		if ((n = recvfrom(sFd, buffer, 2, 0, (struct sockaddr*) &serverAddr, &serverAddrLen)) < 0)
		{
			perror("[ListenSunSpot] Data Reception Error \n");
			break;
		}
		buffer[2] = '\0';
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Buffer length : %s \n", buffer);
        #endif*/
        
		// Convert array of char corresponding to hexadecimal number to int
		tailleTrame = xtoi(buffer);
        
		// One byte corresponds to 2 char
		message = (char*) malloc (tailleTrame * sizeof(char)*2 + 1);
        
		// Reception of the message without the header
		if ((n = recvfrom(sFd, buffer, tailleTrame*2, 0, (struct sockaddr*) &serverAddr, &serverAddrLen)) < 0)
		{
			perror("[ListenSunSpot] Data Reception Error \n");
			break;
		}
        
		message[tailleTrame*2] = '\0';
        
        #if DEBUG > 0
            printf("[ListenSunSpot] Data received : %s \n", message);
        #endif
        
		ManageMessage(message);
	}
    
	close(sFd);
    
	return 0;
}

<<<<<<< HEAD
	/* Internet Protocol */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // on assume que le serveur Java sera local
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

	// on est en UDP, pas de connect à faire, mais un bind
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

		// Convert array of char corresponding to hexadecimal number to int
		tailleTrame = xtoi(buffer);

		// One byte corresponds to 2 char
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		// Reception of the message without the header
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

void *ListenEnOcean(void *ptr)
{
	int sFd;
	char buffer[5], *message;
	long n;
=======
void *ListenEnOcean(void *ptr) {
	int sFd;
	char buffer[5], *message;
    long n;
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32
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
<<<<<<< HEAD
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
=======
		return ERROR;
	}

	#if DEBUG > 0
		printf("[ListenEnOcean] Connection with the server...\n");
	#endif

	if (connect(sFd,(struct sockaddr*)&serverAddr,serverAddrLen) == -1) {
		perror("[ListenEnOcean] Socket connection Error \n");
		return SOCKET_ERROR;
	}

	#if DEBUG > 0
		printf("[ListenEnOcean] Connection with the server OK\n");
	#endif

	while (1){
		#if DEBUG > 0
			printf("[ListenEnOcean] Waiting for a message debut...\n");
		#endif
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32

		while (strcmp(buffer, "A55A") != 0)
		{
			if ((n = recv(sFd, buffer, sizeof(buffer - 1), 0)) < 0)
			{
				perror("[ListenEnOcean] Receive Error \n");
				break;
			}
			buffer[4] = '\0';
		}

<<<<<<< HEAD
#if DEBUG > 0
		printf("[ListenEnOcean] Message received.\n");
#endif
=======
		#if DEBUG > 0
			printf("[ListenEnOcean] Message received.\n");
		#endif
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32

		if ((n = recv(sFd, buffer, 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Data Reception Error \n");
			break;
		}
		buffer[2] = '\0';

<<<<<<< HEAD
#if DEBUG > 0
		printf("[ListenEnOcean] Buffer length : %s \n", buffer);
#endif
=======
		#if DEBUG > 0
			printf("[ListenEnOcean] Buffer length : %s \n", buffer);
		#endif
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32

		// Convert array of char corresponding to hexadecimal number to int
		tailleTrame = xtoi(buffer);

		// One byte corresponds to 2 char
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		// Reception of the message without the header
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

	currentSensor = sensorList;

<<<<<<< HEAD
#if DEBUG > 0
	printf("Message : %s \n", message);
#endif
=======
	#if DEBUG > 0
		printf("Message : %s \n", message);
	#endif
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32

	org = atoi(str_sub(message, 0, 1)); // Extract from the message the type of sensor
	teachIn = atoi(str_sub(message, 8, 9)); // Extract from the message the characters indicating the teach-in message

	// Search of the sensor in the sensors' list
	while (currentSensor != NULL)
	{
		if (strcmp(currentSensor->id, str_sub(message, 10, 17)) == 0)
		{
			printf("Détecteur présent dans la liste ! \n");
			currentSensor->decodeMessage(message, *currentSensor);
			break;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}

	// If the sensor isn't in the sensors' list
	if (currentSensor == NULL)
	{
		// If the message is a teach-in message
		if ((teachIn & 80) == 80)
		{
			currentSensor = (Sensor*) malloc(sizeof(Sensor));
			strcpy(currentSensor->id, str_sub(message, 10, 17));
			switch (org)
			{
<<<<<<< HEAD
			case 5: // RPS Type sensor
				// A GERER
				break;
			case 6: // 1BS Telegram
				if ((atoi(str_sub(message, 2, 3)) >> 2) == 0)
				{ // Func = 00
					if (((atoi(str_sub(message, 2, 3)) & 0) == 3) && ((atoi(
							str_sub(message, 4, 5)) >> 3) == 1))
					{ // Type = 01
						currentSensor->type = SWITCH;
						currentSensor->data = (Switch_Data*) malloc(
								sizeof(Switch_Data));
						// currentSensor->decodeMessage = decodeMessageSwitch;
=======
				case 5:	// RPS Type sensor
					// A GERER
					break;
				case 6:	// 1BS Telegram
					if ( (atoi(str_sub(message,2, 3)) >>2 ) == 0){ // Func = 00
						if ( ( (atoi(str_sub(message,2,3)) & 0) == 3)
								&& ( (atoi(str_sub(message,4,5)) >>3 ) == 1)){	// Type = 01
							currentSensor->type = SWITCH;
							currentSensor->data = (Switch_Data*) malloc(sizeof(Switch_Data));
							// currentSensor->decodeMessage = decodeMessageSwitch;
							currentSensor->next = NULL;
						}
					}
					break;
				case 7:	// 4BS Type Sensor
	//				printf("Test!!! %i \n", xtoi(str_sub(message,2,3)) >> 2);
					if ( (xtoi(str_sub(message,2, 3)) >> 2 ) == 2 ) { // Func = 02, temperature sensor
						currentSensor->type = TEMP;
						currentSensor->data = (Temp_Data*) malloc(sizeof(Temp_Data));
>>>>>>> f75295c2279fa616f321d7fd4fbebfc118bc7f32
						currentSensor->next = NULL;
					}
				}
				break;
			case 7: // 4BS Type Sensor
				// printf("Test!!! %i \n", xtoi(str_sub(message,2,3)) >> 2);
				if ((xtoi(str_sub(message, 2, 3)) >> 2) == 2)
				{ // Func = 02, temperature sensor
					currentSensor->type = TEMP;
					currentSensor->data
							= (Temp_Data*) malloc(sizeof(Temp_Data));
					currentSensor->next = NULL;
					// printf("Test 42!!! %i \n", xtoi(str_sub(message,2,3)) << 5);
					// printf("Test 21!!! %i \n", xtoi(str_sub(message,4,5)) >> 3);
					if (((xtoi(str_sub(message, 2, 3)) << 6) == 0) && ((xtoi(
							str_sub(message, 4, 5)) >> 3) == 5))
					{ // Type = 05
						//currentSensor->decodeMessage = decodeMessageTemp;
					}
				}
				break;
			}
		}
	}

}

float getInfoFromSensor(char message[10]){
	char id[8];
	Sensor* currentSensor;

	currentSensor = sensorList;

	strcpy(id,str_sub(message, 0, 8));

	// Search of the sensor in the sensors' list
	while (currentSensor != NULL)
	{
		if (strcmp(currentSensor->id, id) == 0)
		{
			printf("Détecteur présent dans la liste ! \n");
			currentSensor->decodeMessage(message, *currentSensor);
			break;
		}
		else
		{
			currentSensor = currentSensor->next;
		}
	}

	return (float)0;
}
