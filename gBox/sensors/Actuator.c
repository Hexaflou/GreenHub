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
#include <mqueue.h>

static char idReceptorEnOcean[7];

/*******************************************************
 *
 *		ACTIONS SUR LES ACTIONNEURS
 *
 ******************************************************/


int ActuatorInterfaceInit(char *arg_idReceptorEnOcean){
	strcpy(idReceptorEnOcean,arg_idReceptorEnOcean);
	return OK;
}

/* Lance une action sur un actionneur de courant
 * Le courant est coupé si value = 7, et est en marche quand value = 5
 */
int actionCurrent(float value, struct Actuator * p_actuator, mqd_t smq) {
	char message[29];
	char valueHexa[3];
	char *actuatorRealID;

	sprintf(valueHexa,"%02X",(int)value);

	actuatorRealID = str_sub(p_actuator->id, strlen(p_actuator->id)-2, strlen(p_actuator->id)-1);

	/******* HEADER *******/
	strcpy(message, "A55A"); /* Début d'un message */
	strcat(message, "6B"); /* SYNC_BYTE */
	strcat(message, "05"); /* LENGTH_BYTE */
	/***** FIN HEADER *****/

	/******* DATA BYTE *******/
	strcat(message, valueHexa); /* BYTE 3 */
	strcat(message, "000000"); /* BYTE 2 à 0 */
	/***** FIN DATA BYTE *****/

	/******* ID *******/
	strcat(message, idReceptorEnOcean); /* 3 octets */
	strcat(message, actuatorRealID); /* 1 octet */
	/***** FIN ID *****/

	strcat(message, "30"); /* STATUS BYTE */
	strcat(message, "00\0"); /* CHECKSUM */

	/*strcpy(message, "A55A6B0550000000FF9F1E053000\0"); */ /* (debug) */
	p_actuator->status = value;
	if (value == (float) 5) {
		printf("[Actuator] Action sur l'actionneur de courant : Mise en marche.\n");
	} else {
		printf("[Actuator] Action sur l'actionneur de courant : Extinction.\n");
	}
	mq_send(smq, message, MAX_MQ_SIZE, 0);
	gLogsLog(p_actuator->id, p_actuator->status);
	free(actuatorRealID);
	return 0;
}

/* ACTUELLEMENT SIMULE, PAR MANQUE DE MATERIEL NOUS NE PRENDRONS EN CHARGE QUE
 * LA MISE A JOUR DU CAPTEUR
 */
int actionTemp(float value, struct Actuator * p_actuator, mqd_t smq) {
	/* NON SUPPORTE ACTUELLEMENT (uniquement en simulation) */

	p_actuator->status = value;
	printf("Action sur le thermostat au niveau : %f\n", value);
	gLogsLog(p_actuator->id, p_actuator->status);
	return 0;
}
