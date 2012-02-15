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
#include "creerEEP.h"
#include "GestionConfiguration.h"
#include "creerConfig.h"

void initializeSensorAndEEPListFirst(){

	createConfigSensors();
	initializeEEPListFirst();

}


void initializeSensorAndEEPList(Sensor ** pp_sensorList, EEP* p_EEPList){

	EEP* p_EEPCurrent;
	Sensor* p_sensorCurrent;	
	readConfig("sensors.txt", "eep.txt", pp_sensorList, p_EEPList);
}


#ifdef TODO_SUPPRIMER /* TODO SUPPRIMER ?????? */
void initializeSensorAndEEPListOld(Sensor * p_sensorList, EEP* EEPList){

	char id[8]; 
	char org[3];
	char funct[3]; 
	char type[3];
	int result;
	

	initializeEEPList("eep.txt",EEPList);


	/* Temperature Sensor */
	strcpy(id, "00893360");
	strcpy(org, "07");
	strcpy(funct, "02");
	strcpy(type, "05");	

	AddSensorByEEP(id, p_sensorList, EEPList, org, funct, type);

	/* Switch Sensor */

	strcpy(id, "0021CBE5");
	strcpy(org, "05");
	strcpy(funct, "02");
	strcpy(type, "01");

	AddSensorByEEP(id, p_sensorList, EEPList, org, funct, type);

	/* CONTACT Sensor */

	strcpy(id, "0001B015");
	strcpy(org, "06");
	strcpy(funct, "00");
	strcpy(type, "01");	

	AddSensorByEEP(id, p_sensorList, EEPList, org, funct, type);

	/* CONTACT Sensor */

	strcpy(id, "0001B016");
	strcpy(org, "06");
	strcpy(funct, "00");
	strcpy(type, "01");	

	AddSensorByEEP(id, p_sensorList, EEPList, org, funct, type);

	/* Light and Occupancy Sensor */

	strcpy(id, "00054155");
	strcpy(org, "07");
	strcpy(funct, "08");
	strcpy(type, "01");	

	AddSensorByEEP(id, p_sensorList, EEPList, org, funct, type);

}
#endif
