/*
 * SimulationCapteur.h
 *
 * Created on: 19 janv. 2012
 * Author: nphan
 */

#ifndef SIMULATIONSENSOR_H_
#define SIMULATIONSENSOR_H_


typedef struct ArgSensor
{
	char id[8];
	char eep[6];
}ArgSensor;

void StartSimulationSensor();
void* SimulationSensorTemp(void *);
void* SimulationSensorSwitch(void *);
void* SimulationSensorContact(void *);
void* SimulationSensorLight(void *);
void* SimulationSensorOccupancy(void *);


#endif /* SIMULATIONSENSOR_H_ */
