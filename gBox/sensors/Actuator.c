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

int actionCurrent(float value, struct Actuator * p_actuator, mqd_t smq) {
    char message[29];
    char valueHexa[3];
    char *actuatorRealID;

    printf("value : %f\n",value);

    sprintf(valueHexa,"%X",(((int)value)<<1)|0x01);
    valueHexa[1] = '0';
    valueHexa[2] = '\0';

if (value == (float)1)
	valueHexa[0] = '5';
if (value == (float)0)
	valueHexa[0] = '7';

    printf("ValueHexa : %s\n",valueHexa);

    actuatorRealID = str_sub(p_actuator->id, strlen(p_actuator->id)-2, strlen(p_actuator->id)-1);
    
    /******* HEADER *******/
    strcpy(message, "A55A"); /* Début d'un message */
    strcat(message, "6B"); /* SYNC_BYTE */
    strcat(message, "07"); /* LENGTH_BYTE */
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
    strcat(message, "00"); /* CHECKSUM */

    /*strcpy(message, "A55A6B0550000000FF9F1E053000\0"); */ /* (debug) */
    p_actuator->status = value;
    if (value == (float) 0) {
	    printf("[Actuator] Action sur l'actionneur de courant : Mise en marche.\n");
    } else {
	    printf("[Actuator] Action sur l'actionneur de courant : Extinction.\n");
    }
	printf("MESSAGE : %s\n",message);
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
    	/* 
	char message[29];
	char valueHexa[3];
    
	sprintf(valueHexa,"%X",(((int)value)<<1)|0x01);
	valueHexa[1] = '0';
	valueHexa[2] = '\0';
	strcpy(message, "A55A6B05");
	strcat(message, valueHexa);
	strcat(message, "00");
	strcat(message, "0000");
	strcat(message, "FF9F1E03");
	strcat(message, "30");
	strcat(message, "00");
	*/
	p_actuator->status = value;
	printf("Action sur le thermostat au niveau : %f\n", value);
	/*mq_send(smq, message, MAX_MQ_SIZE, 0);*/
	gLogsLog(p_actuator->id, p_actuator->status);
	return 0;
}
