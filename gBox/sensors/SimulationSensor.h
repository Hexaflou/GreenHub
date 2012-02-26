#ifndef SIMULATIONSENSOR_H_
#define SIMULATIONSENSOR_H_

/***********************************************************************
 *				SimulationSensor.h
 *
 * Interface utilisateur permettant de simuler des capteurs EnOcean.
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Innclusions externes */
#include <mqueue.h>


enum SIMU_TYPE{
	SIMU_TEMP,
	SIMU_SWITCH,
	SIMU_CONTACT,
	SIMU_LIGHT
};

typedef struct ArgSensor
{
	char id[9];
	int sleepingTime;
}ArgSensor;

typedef struct SimuThreadList
{
	pthread_t thread;
	struct SimuThreadList * next;
}SimuThreadList;

/* Lance la simulation de capteurs */
int StartSimulationSensor(mqd_t);

/* Détruit les composants utilisés pour la simulation */
int StopSimulationSensor();


#endif /* SIMULATIONSENSOR_H_ */
