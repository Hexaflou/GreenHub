/*
 * InterfaceComposant.c
 *
 *  Created on: 4 janv. 2012
 *      Author: H4212
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


int ComponentInterface(){
	int sFd;
	char buffer[5], *message;
	int n, tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);


	/* Internet Protocol */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("134.214.105.28");
	serverAddr.sin_port = htons(5000);

	/* TCP Socket Creation */
	if ((sFd = socket (AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("[ComponentInterface] TCP Socket Creation Error \n");
		return ERROR;
	}

	#if __DEBUG__ > 0
		printf("[ComponentInterface] Connection with the server...\n");
	#endif

	if (connect(sFd,(struct sockaddr*)&serverAddr,serverAddrLen) == -1) {
		perror("[ComponentInterface] Socket connection Error \n");
		return SOCKET_ERROR;
	}

	#if __DEBUG__ > 0
		printf("[ComponentInterface] Connection with the server OK\n");
	#endif

	while (1){
		#if __DEBUG__ > 0
			printf("Waiting for a message debut...\n");
		#endif

		while(strcmp (buffer, "A55A") != 0){
			if((n = recv(sFd, buffer, sizeof (buffer - 1), 0)) < 0)
			{
				perror("[ComponentInterface] Receive Error \n");
				break;
			}
			buffer[4] = '\0';
		}

		#if __DEBUG__ > 0
			printf("Message received.\n");
		#endif

		if((n = recv(sFd, buffer, 2, 0)) < 0)
		{
			perror("[ComponentInterface] Data Reception Error \n");
			break;
		}
		buffer[2] = '\0';

		#if __DEBUG__ > 0
			printf("Buffer length : %s \n", buffer);
		#endif

		// Convert array of char corresponding to hexadecimal number to int
		tailleTrame = xtoi(buffer);

		// One byte corresponds to 2 char
		message = (char*) malloc (tailleTrame * sizeof(char)*2 + 1);

		// Reception of the message without the header
		if((n = recv(sFd, message, tailleTrame*2, 0)) < 0)
		{
			perror("[ComponentInterface] Data Reception Error \n");
			break;
		}
		message[tailleTrame*2] = '\0';
		ManageMessage(message);
	}
	close(sFd);
	return 0;
}

void ManageMessage(char* message){
	int org, teachIn;
	Sensor* currentSensor;

	currentSensor = sensorList;

	#if __DEBUG__ > 0
		printf("Message : %s \n", message);
	#endif

	org = atoi(str_sub(message, 0,1));	// Extract from the message the type of sensor
	teachIn = atoi(str_sub(message, 8, 9));	// Extract from the message the characters indicating the teach-in message

	while (currentSensor != NULL){
		if ( strcmp(currentSensor->id,str_sub(message,10, 17)) ){
			printf("Détecteur présent dans la liste ! \n");
			currentSensor->decodeMessage(message, *currentSensor);
			break;
		}else{
			currentSensor = currentSensor->next;
		}
	}
	if (currentSensor == NULL){
		if ((teachIn & 80) == 80){
		currentSensor = (Sensor*) malloc(sizeof(Sensor));
		strcpy(currentSensor->id, str_sub(message, 10, 17));
			switch(org)
			{
				case 5:	// RPS Type sensor
					// A GERER
					break;
				case 6:	// 1BS Telegram
					if ( (atoi(str_sub(message,2, 3)) >>2 ) == 0){ // Func = 00
						if ( ( (atoi(str_sub(message,2,3)) & 0) == 3)
								&& ( (atoi(str_sub(message,4,5)) >>3 ) == 1)){	// Type = 01
							currentSensor->type = SWITCH;
							currentSensor->data = (Switch_Data*) malloc(sizeof(Switch_Data));
							currentSensor->decodeMessage = decodeMessageSwitch;
							currentSensor->next = NULL;
						}
					}
					break;
				case 7:	// 4BS Type Sensor
	//				printf("Test!!! %i \n", xtoi(str_sub(message,2,3)) >> 2);
					if ( (xtoi(str_sub(message,2, 3)) >> 2 ) == 2 ) { // Func = 02, temperature sensor
						currentSensor->type = TEMP;
						currentSensor->data = (Temp_Data*) malloc(sizeof(Temp_Data));
						currentSensor->next = NULL;
	//					printf("Test 42!!! %i \n", xtoi(str_sub(message,2,3)) << 5);
	//					printf("Test 21!!! %i \n", xtoi(str_sub(message,4,5)) >> 3);
						if ( ( (xtoi(str_sub(message,2,3)) << 6 ) == 0)
								&& ( (xtoi(str_sub(message,4,5)) >> 3 ) == 5)){	// Type = 05
							//currentSensor->decodeMessage = decodeMessageTemp;
						}
					}
					break;
			}
		}
	}

}

