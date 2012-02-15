/*
 * GestionConfiguration.h
 *
 * Created on: 1 Fev. 2012
 * Author: Hexaflou
 */

#ifndef GESTIONCONFIGURATION_H_
#define GESTIONCONFIGURATION_H_

#include "../lib/cJSON.h"
#include "Component.h"

cJSON* createCSON(char* id, char* EEP);
void readConfig(char* fileNameSensor, char* fileNameEEP, char* fileNameActuator, Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList);
void writeConfig(char* fileNameSensor,char* fileNameEEP, Sensor * p_sensorList,EEP* p_EEPList);

#endif /* GESTIONCONFIGURATION_H_ */
 
