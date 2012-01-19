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
	char id[8];
	enum SENSOR_TYPE type;
	void * data;
	struct Sensor* next;
	void (*decodeMessage)(char*, struct Sensor);
}Sensor;

typedef struct InputContactElement	// org 06, func 00, type 01
{
        char id[4];
		int close;	//0 if opened, 1 if closed
        char status;
} InputContact;

typedef struct TemperatureHumiditySensorElement	// org 07, func 04, type 01
{
        char id[4];
		int humidity;	// from 0 to 100%
		int temperature;	// from 0 to 40°C
		int temperatureSensorAvailable; // 1 if temperature sensor available
        char status;
} TemperatureHumiditySensor;

typedef struct LightOccupencySensorElement	// org 07, func 08
{
        char id[4];
        int illumination;
        int temperature;
        int range;
        char status;
} LightOccupencySensor;


int saveSensor(char* file);
int loadSensor(char* file);
int addSensor(Sensor sensor);
int removeSensor(char* id);

void decodeMessageTemp(char* message, struct Sensor);
void decodeMessageLightOccup(char* message, struct Sensor);
void decodeMessageContact(char* message, struct Sensor);
void decodeMessageSwitch(char* message, struct Sensor);

int getTemp(char* message, struct Sensor);
int getLight(char* message, struct Sensor);
int getContact(char* message, struct Sensor);
int getSwitch(char* message, struct Sensor);

#endif /* COMPONENT_H_ */
