/*
 * Test.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nphan
 */

#include "Test.h"
#include "Component.h"
#include "ComponentInterface.h"



void initializeSensorList(){

	struct Sensor* currentSensor;

	currentSensor = sensorList;

	// Temperature Sensor
	sensorList = (Sensor*) malloc(sizeof(Sensor));
	currentSensor = sensorList;
	strcpy(currentSensor->id, "00893378");
	currentSensor->type = TEMP;
	currentSensor->data = (Temp_Data*) malloc(sizeof(Temp_Data));
	((Temp_Data*)currentSensor->data)->rangeMin = 0;
	((Temp_Data*)currentSensor->data)->rangeMax = 40;
	((Temp_Data*)currentSensor->data)->temp = 0;
	currentSensor->decodeMessage = decodeMessageTemp;
	currentSensor->next = (Sensor*) malloc(sizeof(Sensor));

	currentSensor = currentSensor->next;

	// Switch Sensor

	strcpy(currentSensor->id, "0021CBE5");
	currentSensor->type = SWITCH;
	currentSensor->data = (Switch_Data*) malloc(sizeof(Switch_Data));
	currentSensor->decodeMessage = decodeMessageSwitch;
	currentSensor->next = NULL;
}
