/*
 * InterfaceComposant.h
 *
 * Created on: 4 janv. 2012
 * Author: nphan
 */

#ifndef COMPONENTINTERFACE_H_
#define COMPONENTINTERFACE_H_

#define SOCKET_ERROR -1
#define ERROR -1
#define DEBUG 1
#define __DEBUG__ 1

#include "Utility.h"

struct Sensor* sensorList;

int ComponentInterface(void);

void *ListenSunSpot(void *ptr);

void *ListenEnOcean(void *ptr);

void ManageMessage(char* message);

float getInfoFromSensor(char message[10]);

#endif /* COMPONENTINTERFACE_H_ */
