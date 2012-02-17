/* Inclusions internes */
#include "SimulationSensor.h"
#include "ComponentInterface.h"
#include "Component.h"
#include "Utility.h"
#include "ComSndReceptorTask.h"

/* Inclusions externes */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <mqueue.h>

static mqd_t smq;

void StartSimulationSensor(mqd_t arg_smq){
	pthread_t thread1, thread2,thread3;
	/* Simulation d un capteur de temperature entre -30°C et 10°C */
	ArgSensor* argSensor,*argSensor2,*argSensor3;
	
	smq = arg_smq;
	
	argSensor=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	argSensor2=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	argSensor3=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	strcpy(argSensor->id,"00893360");
	strcpy(argSensor->eep,"070205");		
	pthread_create(&thread1, NULL, SimulationSensorTemp, (void*) argSensor);
	

	/* Simulation d un capteur de contact */
	strcpy(argSensor2->id,"0001B015");
	strcpy(argSensor2->eep,"060001");		
	pthread_create(&thread2, NULL, SimulationSensorContact, (void*) argSensor2);

	/* Simulation d un capteur d interrupteur */
	strcpy(argSensor3->id,"0021CBE5");
	strcpy(argSensor3->eep,"050201");		
	pthread_create(&thread3, NULL, SimulationSensorSwitch, (void*) argSensor3);
	
	pthread_join(thread1, NULL); 
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL); 


}

void * SimulationSensorTemp(void * p_argSensor)
{
	char message[29];
	int temp;
	char tempHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		temp = rand()%40 + 1;		
		sprintf(tempHexa,"%X",temp);		
		/* Si la temperature est exprime sur un caractere en hexa il va falloir l etaler sur nos deux caracteres hexa */
		if (temp < 16)
		{
			tempHexa[1] = tempHexa[0];
			tempHexa[0] = '0';
			tempHexa[2] = '\0';
		}
		strcpy(message, "A55A0B07");
		strcat(message, "00");
		strcat(message, tempHexa);
		strcat(message, "0000");
		printf("ID du simulateur de capteur de temerature : %s\n",((struct ArgSensor*)p_argSensor)->id);
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "00\0");
		printf("Message du capteur de température : %s\n",message);
		
		mq_send(smq, message, MAX_MQ_SIZE, 0);
	}
}

void * SimulationSensorContact(void * p_argSensor)
{
	char message[29];
	int contact;
	char contactHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		contact = rand()%2;	
		sprintf(contactHexa,"%X",contact);
		contactHexa[1] = contactHexa[0];
		contactHexa[0] = '0';
		contactHexa[2] = '\0';
		strcpy(message, "A55A0B07");		
		strcat(message, contactHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "00");
		printf("Message du capteur de contact : %s\n",message);
		
		mq_send(smq, message, MAX_MQ_SIZE, 0);
	}
}

void * SimulationSensorSwitch(void * p_argSensor)
{
	char message[29];
	int i_switch;
	char switchHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		i_switch = rand()%2;	
		sprintf(switchHexa,"%X",i_switch);		
		switchHexa[1] = switchHexa[0];
		switchHexa[0] = '0';
		switchHexa[2] = '\0';
		strcpy(message, "A55A0B07");		
		strcat(message, switchHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "00");
		printf("Message du capteur d'interrupteur : %s\n",message);
		
		/*mq_send(smq, message, MAX_MQ_SIZE, 0);*/
		
		ActionActuator("0021CBE5aC00\0", B0);		
	}
}



char* CalculateCheckSum(char * message){
	int nbChar, ii;
	int byteSum;
	char byte[2];
	char * byteSumHexa;
	byteSumHexa =(char*)malloc(sizeof(char)*30);
	nbChar = xtoi(str_sub(message,0,1));
	for (ii = 0; ii<nbChar ; ii += 2){
		/* TODO: MARCHE PAS */
		byte[0] = message[ii];
		byte[1] = message[ii+1];
		byteSum = byteSum + xtoi(byte);		
	}
	sprintf(byteSumHexa,"%X",byteSum);
	printf("ByteSumHexa : %s \n",byteSumHexa);
	return byteSumHexa;
}
