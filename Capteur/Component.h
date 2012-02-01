/*
 * Component.h
 *
 *  Created on: 11 janv. 2012
 *      Author: H4212
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

enum SENSOR_TYPE{
	CONTACT,
	LIGHT_OCCUP,
	TEMP,
	SWITCH
};

enum SWITCH_BUTTON{
	A1,
	A0,
	B1,
	B0,
	NO_BUTTON,
	THREE_FOUR
}SWITCH_BUTTON;

enum ENERGY_BOW{
	RELEASED,
	PRESSED
}ENERGY_BOW;

typedef struct Contact_Data{
	int closed;	//0 if opened, 1 if closed
}Contact_Data;

typedef struct Temp_Data{
	// Set the range of the measure (ex : from -40°C to 0°C, from -30°C to 10°C, ...)
	float rangeMin;
	float rangeMax;
	float temp;
}Temp_Data;

typedef struct Switch_Data{
	enum SWITCH_BUTTON switch_position;
	enum ENERGY_BOW energy_bow;
}Switch_Data;

typedef struct Sensor
{
	char id[9];
	enum SENSOR_TYPE type;
	void * data;
	struct Sensor* next;
	int (*decodeMessage)(char*, struct Sensor);
	float (*getValue)(char, struct Sensor);
}Sensor;


int saveSensor(char* file);
int loadSensor(char* file);
int addSensor(Sensor sensor);
int removeSensor(char* id);

int decodeMessageTemp(char* message, struct Sensor);
int decodeMessageLightOccup(char* message, struct Sensor);
int decodeMessageContact(char* message, struct Sensor);
int decodeMessageSwitch(char* message, struct Sensor);

int getTempWithoutRange(char* message);
int getLightLittleSensor(char* message);
int getLightBigSensor(char* message);
Switch_Data getSwitch(char* message);
int getContact(char* message);

float getValueTemp(char c, struct Sensor *sensor);
float getValueContact(char c, struct Sensor *sensor);
float getValueSwitch(char c, struct Sensor *sensor);

#endif /* COMPONENT_H_ */
