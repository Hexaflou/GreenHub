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
 * Edite les fichiers de configuration en fonction de la liste de capteurs.
 */
void writeAllSensorConfig(char* fileNameSensor, Sensor * p_sensorList);

/*
 * Edite les fichiers de configuration en fonction de la liste d'actionneurs.
 */
void writeAllActuatorConfig(char* fileNameActuator, Actuator * p_actuatorList);

/*
 * Ajoute dans le fichier de configuration un capteur.
 */
void writeConfigSensor(char* fileNameSensor, Sensor * p_sensor);

/*
 * Ajoute dans le fichier de configuration un actionneur.
 */
void writeConfigActuator(char* fileNameActuator, Actuator * p_actuator);

/*
 * Détruit la liste de composants.
 */
int destroyComponentList(Sensor*, Actuator*);

#endif /* CONFIGURATION_H_ */

