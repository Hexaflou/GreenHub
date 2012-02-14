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
#define OK 0
#define DEBUG 1

#include "Utility.h"
#include "Component.h"
#include "EEP.h"
#include <semaphore.h>

int ComponentInterface(void *);

void *ListenSunSpot(void *ptr);

void *ListenEnOcean(void *ptr);

void ManageMessage(char* message);

int GetInfoFromSensor(char id[10], float * p_value);
int AddSensor(char id[8], char org[2], char funct[2], char type[2]);

Sensor * getSensorList();
sem_t getSemaphore();

#endif /* COMPONENTINTERFACE_H_ */
