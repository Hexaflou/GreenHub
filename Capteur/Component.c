/*
 * Component.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Component.h"
#include "Utility.h"

//void decodeMessageTemp(char* message, struct Sensor);
//void decodeMessageLightOccup(char* message, struct Sensor);
//void decodeMessageContact(char* message, struct Sensor);
//void decodeMessageSwitch(char* message, struct Sensor);
//


/* Return the temperature from the message
 * The message must correspond to a message sent by a temperature sensor, otherwise the result won't be pertinent.
 */
int getTemp(char* message, struct Sensor sensor){
	int temp;
	temp = xtoi(str_sub(message,6,7));	// Extract from the message the temperature
	temp = temp*( ((Temp_Data*)sensor.data)->rangeMax - ((Temp_Data*)sensor.data)->rangeMin )/250;
	return temp;
}
//
//int getLightLittleSensor(char* message, struct Sensor){
//	int light;
//	light = xtoi(str_sub(message,4,5));
//	light = light
//}
//int getContact(char* message, struct Sensor);
//int getSwitch(char* message, struct Sensor);
