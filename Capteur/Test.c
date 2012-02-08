/*
 * Test.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nphan
 */

#include "Test.h"
#include "Component.h"
#include "ComponentInterface.h"
#include "EEP.h"



void initializeSensorAndEEPList(Sensor ** pp_sensorList, EEP* EEPList){

	char id[8]; 
	char org[3];
	char funct[3]; 
	char type[3];
	int result;

	EEPList = (EEP*)malloc(sizeof(EEP));

	initializeEEPList(EEPList);


	/* Temperature Sensor */
	strcpy(id, "00893378");
	strcpy(org, "07\0");
	strcpy(funct, "02\0");
	strcpy(type, "05\0");	

	AddSensorByEEP(id, pp_sensorList, EEPList, org, funct, type);

	/* Switch Sensor */

	strcpy(id, "0021CBE5");
	strcpy(org, "05");
	strcpy(funct, "02");
	strcpy(type, "01");

	AddSensorByEEP(id, pp_sensorList, EEPList, org, funct, type);

	/* CONTACT Sensor */

	strcpy(id, "0001B015");
	strcpy(org, "06");
	strcpy(funct, "00");
	strcpy(type, "01");	

	AddSensorByEEP(id, pp_sensorList, EEPList, org, funct, type);

}
