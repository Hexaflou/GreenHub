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

enum SWITCH_POSITION{
	A0,
	A1,
	B0,
	B1
}SWITCH_POSITION;

typedef struct Contact_Data{
	int close;	//0 if opened, 1 if closed
}Contact_Data;

typedef struct Temp_Data{
	// Set the range of the measure (ex : from -40°C to 0°C, from -30°C to 10°C, ...)
	int rangeMin;
	int rangeMax;
	int temp;
}Temp_Data;

typedef struct Switch_Data{
	enum SWITCH_POSITION switch_position;
}Switch_Data;

typedef struct Sensor
{
	char id[9];
	enum SENSOR_TYPE type;
	void * data;
	struct Sensor* next;
	int (*decodeMessage)(char*, struct Sensor);
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
int getLight(char* message);
int getContact(char* message);
int getSwitch(char* message);

#endif /* COMPONENT_H_ */
