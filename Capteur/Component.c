/*
 * Component.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Component.h"
#include "Utility.h"
#include "gLogs.h"

int addSensorsTemp(char id[8], Sensor *sensorList){
	struct Sensor *sensor;
	sensor = (Sensor*) malloc (sizeof(Sensor));	
	return 0;
}

int decodeMessageTemp(char* message, struct Sensor sensor)
{
	float temp;
	temp = (float) getTempWithoutRange(message);
	temp = temp * (((Range*) sensor.rangeData)->rangeMax
			- ((Range*) sensor.rangeData)->rangeMin) / 250;
	if (sensor.value != temp)
	{
		sensor.value = temp;
		printf("Value change ! \n");
		printf("Sensor value : %f \n", sensor.value);
		return 1;
	}
	else
	{
		return 0;
	}
}
/*int decodeMessageLightOccup(char* message, struct Sensor);*/
int decodeMessageContact(char* message, struct Sensor sensor)
{
	int closed;
	closed = getContact(message);
	/*printf("Valeur contact : %i \n", closed);*/
	if (sensor.value != closed)
	{
		sensor.value = getContact(message);
		if (closed == 1){
			printf("Contact closed. \n");
		}else{
			printf("Contact opened. \n");
		}
	}
	return 1;
}

int decodeMessageSwitch(char* message, struct Sensor sensor)
{
	int switch_button = getSwitch(message);
	if (switch_button != NO_BUTTON)
	{		
		sensor.value = switch_button;
		printf("Switch value : %f \n",
				sensor.value);		
		return 1;
	}
	else
	{
		return 0;
	}
}



/* Return the temperature from the message
 * The message must correspond to a message sent by a temperature sensor, otherwise the result won't be pertinent.
 */
int getTempWithoutRange(char* message)
{
	int temp;
	temp = xtoi(str_sub(message, 6, 7)); /* Extract from the message the temperature */
	return temp;
}

/* Return the illumination value from the message
 * The message must correspond to a message sent by a illumination sensor which recorded the value into 1 byte,
 * otherwise the result won't be pertinent.
 */
int getLightLittleSensor(char* message)
{
	int light;
	light = xtoi(str_sub(message, 4, 5)); /* Extract from the message the temperature */
	return light;
}

/* Return the illumination value from the message
 * The message must correspond to a message sent by a illumination sensor which recorded the value into 2 byte,
 * otherwise the result won't be pertinent.
 */
int getLightBigSensor(char* message)
{
	int light;
	light = xtoi(str_sub(message, 4, 7)); /* Extract from the message the temperature */
	return light;
}

/* Return the contact value from the message
 * The message must correspond to a message sent by a illumination sensor which recorded the value into 2 byte,
 * otherwise the result won't be pertinent.
 */
int getSwitch(char* message)
{
	int result;
	int byte, status;
	byte = xtoi(str_sub(message, 2, 3)); /* Extract from the message the data byte */
	status = xtoi(str_sub(message, 18, 19)); /* Extract from the message the status byte */
	if (status & (1u << 4))
	{ /* If the message is a N-message type */
		result = ((byte & 0xE0) >> 5); /* Extract the 5 to 7 bit */
	}
	else
	{ /* If the message is a U-message type */
		if (((byte & 0xE0) >> 5) == 0)
		{
			result = NO_BUTTON;
		}
		else if (((byte & 0xE0) >> 5) == 3)
		{
			result = THREE_FOUR;
		}
	}
	return (int)result;
}

int getContact(char* message)
{
	int closed, byte;
	byte = xtoi(str_sub(message, 8, 9)); /* Extract from the message the contact byte */
	closed = byte & 0x01; /* Extract the bit 0 from the byte */
	return closed;
}
