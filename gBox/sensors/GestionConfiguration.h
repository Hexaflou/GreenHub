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
void readConfig(char* fileNameSensor, char* fileNameEEP, Sensor ** p_sensorList, EEP* EEPList);
void rewriteConfig(char* fileName, Sensor * p_sensorList);

#endif /* GESTIONCONFIGURATION_H_ */
 
