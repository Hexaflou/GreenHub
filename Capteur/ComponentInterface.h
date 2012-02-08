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
#define DEBUG 1
#define __DEBUG__ 1

#include "Utility.h"
#include "Component.h"
#include "EEP.h"

int initializeEEPList(EEP*);

int ComponentInterface(void);

void *ListenSunSpot(void *ptr);

void *ListenEnOcean(void *ptr);

void ManageMessage(char* message, Sensor * p_sensorList);

float GetInfoFromSensor(char message[10], Sensor * p_sensorList);
int AddSensor(char id[8], char org[2], char funct[2], char type[2]);

#endif /* COMPONENTINTERFACE_H_ */
