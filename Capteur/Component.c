/*
 * Component.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Component.h"
#include "Utility.h"
#include "gLogs.h"

int decodeMessageTemp(char* message, struct Sensor sensor)
{
	float temp;
	temp = (float) getTempWithoutRange(message);
	temp = temp * (((Temp_Data*) sensor.data)->rangeMax
			- ((Temp_Data*) sensor.data)->rangeMin) / 250;
	if (((Temp_Data*) sensor.data)->temp != temp)
	{
		((Temp_Data*) sensor.data)->temp = temp;
		gLogsLog(sensor.id, ((Temp_Data*) sensor.data)->temp);
		printf("Value change ! \n");
		printf("Sensor value : %f \n", ((Temp_Data*) sensor.data)->temp);
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
	printf("Valeur contact : %i \n", closed);
	if (((Contact_Data*) sensor.data)->closed != closed)
	{
		( (Contact_Data*) sensor.data)->closed = getContact(message);
		if (closed == 1){
			printf("Contact closed. \n");
		}else{
			printf("Contact opened. \n");
		}
		gLogsLog(sensor.id, ( (Contact_Data*) sensor.data)->closed);
	}
	return 1;
}

int decodeMessageSwitch(char* message, struct Sensor sensor)
{
	Switch_Data switch_data = getSwitch(message);
	if (switch_data.switch_position != NO_BUTTON)
	{
		((Switch_Data*) sensor.data)->energy_bow = switch_data.energy_bow;
		((Switch_Data*) sensor.data)->switch_position
				= switch_data.switch_position;
		printf("Switch value : %i \n",
				((Switch_Data*) sensor.data)->switch_position);		
		gLogsLog(sensor.id, ((Switch_Data*) sensor.data)->switch_position);
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
Switch_Data getSwitch(char* message)
{
	Switch_Data result;
	int byte, status;
	byte = xtoi(str_sub(message, 2, 3)); /* Extract from the message the data byte */
	status = xtoi(str_sub(message, 18, 19)); /* Extract from the message the status byte */
	if (status & (1u << 4))
	{ /* If the message is a N-message type */
		result.switch_position = ((byte & 0xE0) >> 5); /* Extract the 5 to 7 bit */
	}
	else
	{ /* If the message is a U-message type */
		if (((byte & 0xE0) >> 5) == 0)
		{
			result.switch_position = NO_BUTTON;
		}
		else if (((byte & 0xE0) >> 5) == 3)
		{
			result.switch_position = THREE_FOUR;
		}
	}
	result.energy_bow = byte & (0x01 << 4); /* Extract the bit 4 from the byte */
	return result;
}

int getContact(char* message)
{
	int closed, byte;
	byte = xtoi(str_sub(message, 8, 9)); /* Extract from the message the contact byte */
	closed = byte & 0x01; /* Extract the bit 0 from the byte */
	return closed;
}

float getValueTemp(char c, struct Sensor *sensor){
	return ( (Temp_Data*)sensor->data)->temp;
}

float getValueContact(char c, struct Sensor *sensor){
	return ( (Contact_Data*)sensor->data)->closed;
}

float getValueSwitch(char c, struct Sensor *sensor){
	return ((Switch_Data*)sensor->data)->switch_position;
}
