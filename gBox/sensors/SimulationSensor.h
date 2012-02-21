/*
 * SimulationCapteur.h
 *
 * Created on: 19 janv. 2012
 * Author: nphan
 */

#ifndef SIMULATIONSENSOR_H_
#define SIMULATIONSENSOR_H_

#include <mqueue.h>

typedef struct ArgSensor
{
	char id[9];
	char eep[7];
}ArgSensor;

/* Calcule la check sum d'un message. Les caractères "A55A" ne doivent être pas présents. */
char* CalculateCheckSum(char * message);

void* StartSimulationSensor(void *);
void* SimulationSensorTemp(void *);
void* SimulationSensorSwitch(void *);
void* SimulationSensorContact(void *);
void* SimulationSensorLight(void *);
void* SimulationSensorOccupancy(void *);


#endif /* SIMULATIONSENSOR_H_ */
