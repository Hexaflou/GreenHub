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


void initializeConfig(Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* p_EEPList){

	readConfig("sensors.txt", "eep.txt", "actuators.txt", pp_sensorList, pp_actuatorList, p_EEPList);

	/*writeConfig("sensorsWrite.txt", "eepWrite.txt", *pp_sensorList, p_EEPList);*/

}
