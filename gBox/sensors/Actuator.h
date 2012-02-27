#ifndef ACTUATOR_H_ 
#define ACTUATOR_H_

/***********************************************************************
 *				Actuator.h
 *
 * Fichier contenant les structures et les fonctions permettant
 * d'exploiter les actionneurs EnOcean.
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions externes */
#include <mqueue.h>

typedef struct ActuatorRange{
	float rangeMin;
	float rangeMax;
}ActuatorRange;

typedef struct Actuator
{
	char id[12];	/* ID of the synchronized sensor + 'a' + ID of the actuator + '\0' */
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
