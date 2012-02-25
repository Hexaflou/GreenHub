/*
 * Configuration.h
 *
 * Created on: 1 Fev. 2012
 * Author: Hexaflou
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "../lib/cJSON.h"
#include "Sensor.h"
#include "Actuator.h"

cJSON* createCSON(char* id, char* EEP);
void readConfig(char* fileNameSensor, char* fileNameEEP, char* fileNameActuator, Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList);
void writeConfig(char* fileNameSensor,char* fileNameEEP, Sensor * p_sensorList,EEP* p_EEPList);
int destroyComponentList(Sensor*, Actuator*);

#endif /* CONFIGURATION_H_ */
 
