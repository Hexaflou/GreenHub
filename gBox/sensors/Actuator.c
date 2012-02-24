/*
 * Actuator.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Actuator.h"
#include "Utility.h"
#include "../gLogs.h"
#include "ComponentInterface.h"

/*******************************************************
 *
 *		ACTIONS SUR LES ACTIONNEURS
 *
 ******************************************************/

int actionCurrent(float value, struct Actuator * p_actuator, mqd_t smq){
	char message[29];
	int i_switch;
	char valueHexa[3];

	/*printf("value : %f\n",value);

	sprintf(valueHexa,"%X",(((int)value)<<1)|0x01);
	valueHexa[1] = '0';
	valueHexa[2] = '\0';

	printf("ValueHexa : %s\n",valueHexa);

	strcpy(message, "A55A6B05");
	strcat(message, valueHexa);
	strcat(message, "00");
	strcat(message, "0000");
	strcat(message, "FF9F1E03");
	strcat(message, "30");
	strcat(message, "00");	*/
	strcpy(message,"A55A6B0550000000FF9F1E033000\0");
	/*printf("Message du capteur d'interrupteur : %s\n",message);*/
	mq_send(smq, message, MAX_MQ_SIZE, 0);
	return 0;
}

int actionTemp(float value, struct Actuator * p_actuator, mqd_t smq){
	char message[29];
	int i_switch;
	char valueHexa[3];

	/*printf("value : %f\n",value);

	sprintf(valueHexa,"%X",(((int)value)<<1)|0x01);
	valueHexa[1] = '0';
	valueHexa[2] = '\0';

	printf("ValueHexa : %s\n",valueHexa);

	strcpy(message, "A55A6B05");
	strcat(message, valueHexa);
	strcat(message, "00");
	strcat(message, "0000");
	strcat(message, "FF9F1E03");
	strcat(message, "30");
	strcat(message, "00");	*/
	strcpy(message,"A55A6B0550000000FF9F1E033000\0");
	/*printf("Message du capteur d'interrupteur : %s\n",message);*/
	mq_send(smq, message, MAX_MQ_SIZE, 0);
	return 0;
}
