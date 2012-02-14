/* Inclusions internes */
#include "SimulationSensor.h"
#include "ComponentInterface.h"
#include "Utility.h"

/* Inclusions externes */
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


void StartSimulationSensor(){
	pthread_t thread1, thread2,thread3;
	/* Simulation d un capteur de temperature entre -30°C et 10°C */
	ArgSensor* argSensor,*argSensor2,*argSensor3;
	argSensor=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	argSensor2=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	argSensor3=(struct ArgSensor*)malloc(sizeof(struct ArgSensor));
	strcpy(argSensor->id,"06080501");
	strcpy(argSensor->eep,"070202");		
	pthread_create(&thread1, NULL, SimulationSensorTemp, (void*) argSensor);
	

	/* Simulation d un capteur de contact */
	strcpy(argSensor2->id,"06888881");
	strcpy(argSensor2->eep,"060001");		
	pthread_create(&thread2, NULL, SimulationSensorContact, (void*) argSensor2);

	/* Simulation d un capteur d interrupteur */
	strcpy(argSensor3->id,"06555555");
	strcpy(argSensor3->eep,"050201");		
	pthread_create(&thread3, NULL, SimulationSensorSwitch, (void*) argSensor3);
	
	pthread_join(thread1, NULL); 
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL); 


}

void * SimulationSensorTemp(void * p_argSensor)
{
	char message[28];
	int temp;
	char tempHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		temp = rand()%40 + 1;	
		sprintf(tempHexa,"%X",temp);		
		strcpy(message, "A55A0B07");
		strcat(message, "00");
		strcat(message, tempHexa);
		strcat(message, "0000");
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "11");
		printf("Message du capteur de température : %s\n",message);
	}
}

void * SimulationSensorContact(void * p_argSensor)
{
	char message[28];
	int contact;
	char contactHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		contact = rand()%2;	
		sprintf(contactHexa,"%X",contact);		
		strcpy(message, "A55A0B07");		
		strcat(message, contactHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "11");
		printf("Message du capteur de contact : %s\n",message);
	}
}

void * SimulationSensorSwitch(void * p_argSensor)
{
	char message[28];
	int i_switch;
	char switchHexa[3];
	
	srand(time(NULL));
	AddSensor(((struct ArgSensor*)p_argSensor)->id, str_sub(((struct ArgSensor*)p_argSensor)->eep, 0, 1), str_sub(((struct ArgSensor*)p_argSensor)->eep, 2, 3), str_sub(((struct ArgSensor*)p_argSensor)->eep, 4, 5));
	while (1){
		sleep(5);
		i_switch = rand()%2;	
		sprintf(switchHexa,"%X",i_switch);		
		strcpy(message, "A55A0B07");		
		strcat(message, switchHexa);
		strcat(message, "00");
		strcat(message, "0000");
		strcat(message, ((struct ArgSensor*)p_argSensor)->id);
		strcat(message, "00");
		strcat(message, "11");
		printf("Message du capteur d'interrupteur : %s\n",message);
	}
}