/*
 * EEP.h
 *
 * Created on: 1 Fev. 2012
 * Author: Hexaflou
 */

#ifndef EEP_H_
#define EEP_H_

#include "Component.h"

typedef struct EEP
{
	char eep[6];
	int (*AddSensors)(char id[8], Sensor ** pp_sensor, float arg1, float arg2);
	char name[70];
	struct EEP* next;
	/* Optionnal arguments to specify the range of the measure */	
	int arg1;
	int arg2;	
}EEP;

int initializeEEPList(EEP*);
int destroyEEPList(EEP*);

int AddSensorByEEP(char id[8], Sensor ** pp_sensorList, EEP* p_EEPList, char org[3], char funct[3], char type[3]);

int AddSensorsContact(char id[8], Sensor ** pp_sensorList, float arg1, float arg2);
int AddSensorsSwitch(char id[8], Sensor ** pp_sensorList, float arg1, float arg2);
int AddSensorsTemp(char id[8], Sensor ** pp_sensorList, float arg1, float arg2);

#endif /* COMPONENTINTERFACE_H_ */
 
