/*
 * Actuator.h
 *
 *  Created on: 11 janv. 2012
 *      Author: H4212
 */

#ifndef ACTUATOR_H_ 
#define ACTUATOR_H_

#include <mqueue.h>

typedef struct ActuatorRange{
	float rangeMin;
	float rangeMax;
}ActuatorRange;

typedef struct Actuator
{
	char id[13];
	char EEP[7];
	float status;
	ActuatorRange * rangeData;
	int (*action)(float value, struct Actuator * actuator, mqd_t smq);
	struct Actuator * next;
}Actuator;


/* Fonctions d actions pour les actionneurs */
int actionCurrent(float value, struct Actuator * actuator, mqd_t smq);
int actionTemp(float value, struct Actuator * actuator, mqd_t smq);

#endif /* ACTUATOR_H_ */
