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
	char eep[7];
	int (*AddComponent)(char* id, void ** pp_ComponentList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
	char name[70];
	struct EEP* next;
	/* Optionnal arguments to specify the range of the measure */	
	int scaleMin;
	int scaleMax;	
	int rangeMin;
	int rangeMax;
}EEP;

int initializeEEPList(char*,EEP*);
int writeEEPList(char* fileNameEEP, EEP* p_EEPList);


int AddComponentByEEP(char * id, void ** pp_componentList, EEP* p_EEPList, char org[3], char funct[3], char type[3]);


int AddSensorContact(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorSwitch(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorTemp(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorLightOccupancy(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorTempLightSunSpot(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);

int AddActuatorCurrent(char * id, void ** pp_ActuatorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);

#endif /* COMPONENTINTERFACE_H_ */
 
