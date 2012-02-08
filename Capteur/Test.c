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

	/* Temperature Sensor */
	sensorList = (Sensor*) malloc(sizeof(Sensor));
	currentSensor = sensorList;
	strcpy(currentSensor->id, "e00893378");
	currentSensor->type = TEMP;
	currentSensor->data = (Temp_Data*) malloc(sizeof(Temp_Data));
	((Temp_Data*)currentSensor->data)->rangeMin = 0;
	((Temp_Data*)currentSensor->data)->rangeMax = 40;
	((Temp_Data*)currentSensor->data)->temp = 0;
	currentSensor->decodeMessage = decodeMessageTemp;
	currentSensor->getValue = getValueTemp;
	currentSensor->next = (Sensor*) malloc(sizeof(Sensor));

	currentSensor = currentSensor->next;

	/* Switch Sensor */

	strcpy(currentSensor->id, "e0021CBE5");
	currentSensor->type = SWITCH;
	currentSensor->data = (Switch_Data*) malloc(sizeof(Switch_Data));
	currentSensor->decodeMessage = decodeMessageSwitch;
	currentSensor->getValue = getValueSwitch;
	currentSensor->next = (Sensor*) malloc(sizeof(Sensor));

	currentSensor = currentSensor->next;

	/* CONTACT Sensor */

	strcpy(currentSensor->id, "e0001B015");
	currentSensor->type = CONTACT;
	currentSensor->data = (Contact_Data*) malloc(sizeof(Contact_Data));
	((Contact_Data*)currentSensor->data)->closed = 0;
	currentSensor->decodeMessage = decodeMessageContact;
	currentSensor->getValue = getValueContact;
	currentSensor->next = NULL;
}
