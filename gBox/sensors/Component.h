/*
 * Component.h
 *
 *  Created on: 11 janv. 2012
 *      Author: H4212
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <mqueue.h>

#define A1 0
#define A0 1
#define B1 2
#define B0 3
#define NO_BUTTON 4
#define THREE_FOUR 5
#define VALUE_CHANGE 0
#define NO_CHANGE 1

typedef struct Range{
	/* Set the range of the measure (ex : from -40°C to 0°C, from -30°C to 10°C, ...) */
	float scaleMin;
	float scaleMax;
	float rangeMin;
	float rangeMax;
}Range;

typedef struct Sensor
{
	char id[11];
	char EEP[7];
	Range * rangeData;
	float value;	
	int (*decodeMessage)(char*, struct Sensor*);
	struct Sensor* next;
}Sensor;

typedef struct Actuator
{
	char id[13];
	char EEP[7];
	float status;
	int (*action)(float value, struct Actuator * actuator, mqd_t smq);
	struct Actuator * next;
}Actuator;

/* Add sensor to the sensors' list. If the sensor can get several measures, the function creates as many sensors as measures. 
** Returns 0 if the addition is successful, 1 if there's an error	
*/

/* Fonctions de decodage pour les capteurs */
int decodeMessageTemp(char* message, struct Sensor *);
int decodeMessageLightOccup(char* message, struct Sensor *);
int decodeMessageContact(char* message, struct Sensor *);
int decodeMessageSwitch(char* message, struct Sensor *);
int decodeMessageOccupancy(char * message, struct Sensor *);
int decodeMessageLight(char * message, struct Sensor *);

/* Fonctions de retrait de donnees des trames en parametre */
int getTempWithoutRange(char* message);
int getLightLittleSensor(char* message);
int getLightBigSensor(char* message);
int getSwitch(char* message);
int getOccupancy(char* message);
int getContact(char* message);

/* Fonctions d actions pour les actionneurs */
int actionCurrent(float value, struct Actuator * actuator, mqd_t smq);

#endif /* COMPONENT_H_ */
