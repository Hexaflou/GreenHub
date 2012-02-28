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


/*******************************************************
 *
 *		ACTIONS SUR LES ACTIONNEURS
 *
 ******************************************************/


int actionCurrent(float value, struct Actuator * p_actuator, mqd_t smq) {
    char message[29];
    char valueHexa[3];

    printf("value : %f\n",value);

    sprintf(valueHexa,"%X",(((int)value)<<1)|0x01);
    valueHexa[1] = '0';
    valueHexa[2] = '\0';

    printf("ValueHexa : %s\n",valueHexa);

    strcpy(message, "A55A6B05");	/* HEADER */
    strcat(message, valueHexa);		/* Valeur à transmettre */
    strcat(message, "000000");		/* Byte 2 à 0 */     
    strcat(message, "FF9F1E03");	/* ID du récepteur + numéro de l'actionneur */
    strcat(message, "30");		/* Status */
    strcat(message, "00\0");		/* CheckSum à 00 */
    /*strcpy(message, "A55A6B0550000000FF9F1E053000\0"); */ /* (debug) */
    p_actuator->status = value;
    if (value == (float) 0) {
        printf("[Actuator] Action sur l'actionneur de courant : Mise en marche.\n");
    } else {
        printf("[Actuator] Action sur l'actionneur de courant : Extinction.\n");
    }
    mq_send(smq, message, MAX_MQ_SIZE, 0);
    gLogsLog(p_actuator->id, p_actuator->status);
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
