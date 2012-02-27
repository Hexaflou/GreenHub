#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/***********************************************************************
 *			Configuration.h
 *
 * Interface permettant de configurer la liste des composants et EEP
 * en fonction des fichiers de configuration.
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions internes */
#include "../lib/cJSON.h"
#include "Sensor.h"
#include "Actuator.h"

cJSON* createCSON(char* id, char* EEP);

/*
 * Configure la liste des composants et EEP en fonction des fichiers de configuration
 * dont les chemins sont donnés en paramètre.
 * Renvoie OK si tout se passe bien, ERROR s'il y a eu un problème dans la lecture des fichiers de configuration.
 */
int readConfig(char* fileNameSensor, char* fileNameEEP, char* fileNameActuator, Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList);

/*
 * Edite les fichiers de configuration en fonction de la liste de composants et EEP
 */
void writeConfig(char* fileNameSensor, char* fileNameEEP, Sensor * p_sensorList, EEP* p_EEPList);

/*
 * Détruit la liste de composants.
 */
int destroyComponentList(Sensor*, Actuator*);

#endif /* CONFIGURATION_H_ */

