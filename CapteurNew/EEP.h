/*
 * EEP.h
 *
 * Created on: 1 Fev. 2012
 * Author: Hexaflou
 */

#ifndef EEP_H_
#define EEP_H_

#include "Component.h"

#define NOT_SUPPORTED -1
#define NOT_FOUND -2

typedef struct EEP
{
	char eep[6];
	int (*AddSensors)(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax);
	char name[70];
	struct EEP* next;
	/* Optionnal arguments to specify the range of the measure */	
	int scaleMin;
	int scaleMax;	
	int rangeMin;
	int rangeMax;
}EEP;

int initializeEEPList(char*,EEP*);

int AddSensorByEEP(char id[8], Sensor ** pp_sensorList, EEP* p_EEPList, char org[3], char funct[3], char type[3]);

int AddSensorsContact(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorsSwitch(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorsTemp(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorsLightOccupancy(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax);


#endif /* COMPONENTINTERFACE_H_ */
 
