/*
 * InterfaceComposant.h
 *
 * Created on: 4 janv. 2012
 * Author: Hexaflou
 */

#ifndef COMPONENTINTERFACE_H_
#define COMPONENTINTERFACE_H_

#define SOCKET_ERROR -1
#define ERROR -1
#define COMPONENT_ALREADY_EXIST -3
#define INVALID_ID -2
#define OK 0
#define DEBUG 0
#define MAX_MQ_SIZE    1024


#include "Utility.h"
#include "Sensor.h"
#include "Actuator.h"
#include "EEP.h"
#include <semaphore.h>

int ComponentInterfaceInit();
int ComponentInterfaceClose();

void ManageMessage(char* message);

int GetInfoFromSensor(char id[10], float * p_value);
int AddComponent(char * id, char org[2], char funct[2], char type[2]);

int AddActuator(char id[8], char org[2], char funct[2], char type[2]);
int GetStatusFromActuator(char *id, float * p_value);
int ActionActuator(char *id, float value);

Sensor * getSensorList();
Actuator * getActuatorList();
sem_t getSemSensor();
sem_t getSemActuator();

#endif /* COMPONENTINTERFACE_H_ */
