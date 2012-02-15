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


void initializeConfig(Sensor ** pp_sensorList, EEP* p_EEPList){

	EEP* p_EEPCurrent;
	Sensor* p_sensorCurrent;	
	readConfig("sensors.txt", "eep.txt", pp_sensorList, p_EEPList);
}
