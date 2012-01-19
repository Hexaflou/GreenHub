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
	sensorList = (Sensor*) malloc(sizeof(Sensor));
	strcpy(sensorList->id, "00893360");
	sensorList->type = TEMP;
	sensorList->data = (Temp_Data*) malloc(sizeof(Temp_Data));

	((Temp_Data*)sensorList->data)->rangeMin = -40;
	((Temp_Data*)sensorList->data)->rangeMax = 0;
	((Temp_Data*)sensorList->data)->temp = 0;
	sensorList->decodeMessage = decodeMessageTemp;
	sensorList->next = NULL;
}
