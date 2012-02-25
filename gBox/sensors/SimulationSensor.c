/* Inclusions internes */
#include "SimulationSensor.h"
#include "ComponentInterface.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Utility.h"
#include "ComReceptorTask.h"

/* Inclusions externes */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <mqueue.h>


/***************************PRIVATE DECLARATION***********************/
static void *SimulationSensorTemp(void *ptr);
static void *SimulationSensorContact(void *ptr);
static void *SimulationSensorSwitch(void *ptr);

static mqd_t smq;
static pthread_t thread1,thread2,thread3;
static ArgSensor* argSensor,*argSensor2,*argSensor3;

/************************PUBLIC***************************************/

/* Fonction permettant de lancer la simulation de plusieurs capteurs. */
int StartSimulationSensor(mqd_t arg_smq){
	/* Simulation d un capteur de temperature entre 0°C et 40°C */	
	/*smq = *((mqd_t*)arg_smq);*/
	smq = arg_smq;
	
	srand(time(NULL));

	/* Ces structures seront désallouées par les tâches crées plus bas */
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
	return 0;
}

int StopSimulationSensor()
{
	int ret1,ret2,ret3;
	ret1 = pthread_cancel(thread1);
	ret2 = pthread_cancel(thread2);
	ret3 = pthread_cancel(thread3);
	return (ret1||ret2||ret3);
}

/* Tâche simulant un capteur de température, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID et EEP du capteur.
 */
void * SimulationSensorTemp(void * p_argSensor)
{
	char message[29];
	int temp;
	char tempHexa[3];
	char id[9];

	strcpy(id,((struct ArgSensor*)p_argSensor)->id);
	free(p_argSensor);
	
/*	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
	while (1){
		sleep(rand()%10);
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
		strcat(message, "0000");
		strcat(message, tempHexa);
		strcat(message, "00");
		strcat(message, id);
		strcat(message, "00");
		strcat(message, "00");
/*		printf("Message du capteur de température : %s\n",message);*/

		
		mq_send(smq, message, MAX_MQ_SIZE, 0);
	}

	return (void*)NULL;
}

/* Tâche simulant un capteur de contact, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID et EEP du capteur.
 */
void * SimulationSensorContact(void * p_argSensor)
{
	char message[29];
	int contact;
	char contactHexa[3];
	char id[9];
	
	strcpy(id,((struct ArgSensor*)p_argSensor)->id);

	free(p_argSensor);
	
	/*AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
	while (1){
		sleep(rand()%10);
		contact = rand()%2;	
		sprintf(contactHexa,"%X",contact);
		contactHexa[1] = contactHexa[0];
		contactHexa[0] = '0';
		contactHexa[2] = '\0';
		strcpy(message, "A55A0B07");
		strcat(message, contactHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, id);
		strcat(message, "00");
		strcat(message, "00");
		/*printf("Message du capteur de contact : %s\n",message);*/

		mq_send(smq, message, MAX_MQ_SIZE, 0);
	}
	return (void*)NULL;
}


/* Tâche simulant un interrupteur à 4 etats, de l'ajout de celui-ci jusqu'à l'envoi de trames à partir de ces capteurs.
 * p_argSensor : Structure argument contenant les informations ID et EEP du capteur.
 */
void * SimulationSensorSwitch(void * p_argSensor)
{
	char message[29];
	int i_switch;
	char switchHexa[3];
	char id[9];
	
	strcpy(id,((struct ArgSensor*)p_argSensor)->id);
	free(p_argSensor);

/*	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));*/
	while (1){
		sleep(rand()%10);
		i_switch = (rand()%4)<<1;
		sprintf(switchHexa,"%X",i_switch);
		switchHexa[1] = '0';
		switchHexa[2] = '\0';
		strcpy(message, "A55A0B07");
		strcat(message, switchHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, id);
		strcat(message, "30");
		strcat(message, "00");
	/*	printf("Message du capteur d'interrupteur : %s\n",message);*/

		mq_send(smq, message, MAX_MQ_SIZE, 0);		
	}
	return (void*)NULL;
}


/* Fonction permettant de calculer la checksum d'un message. Le champ checksum du message en paramètre sera mis à jour.
 * Paramètre d'entrée :char * message : trame à traiter.
 */
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
