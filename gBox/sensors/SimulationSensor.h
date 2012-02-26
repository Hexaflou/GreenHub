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

int StartSimulationSensor(mqd_t);
int StopSimulationSensor();


#endif /* SIMULATIONSENSOR_H_ */
