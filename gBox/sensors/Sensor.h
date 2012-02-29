#ifndef SENSOR_H_
#define SENSOR_H_

/***********************************************************************
 *				Sensor.h
 *
 * Interface utilisateur permettant d'exploiter les
 * capteurs SunSpot/EnOcean.
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions externes */
#include <mqueue.h>

/* Définition de constantes */
#define A1 0
#define A0 1
#define B1 2
#define B0 3
#define NO_BUTTON 4
#define THREE_FOUR 5
#define VALUE_CHANGE 0
#define NO_CHANGE 1

typedef struct SensorRange {
	/* Set the range of the measure (ex : from -40°C to 0°C, from -30°C to 10°C, ...) */
	float scaleMin;
	float scaleMax;
	float rangeMin;
	float rangeMax;
} SensorRange;

typedef struct Sensor {
	char id[11]; /* Real ID of the sensor + 's'|'e'(SunSpot|EnOcean) + 'L'|'T'|... (Light|Temperature|...) + '\0'*/
	char EEP[7];
	SensorRange * rangeData;
	float value;
	int (*decodeMessage)(char*, struct Sensor*);
	struct Sensor* next;
} Sensor;

/* Fonctions de decodage pour les capteurs */
int decodeMessageTemp(char* message, struct Sensor *);
int decodeMessageLightOccup(char* message, struct Sensor *);
int decodeMessageContact(char* message, struct Sensor *);
int decodeMessageSwitch(char* message, struct Sensor *);
int decodeMessageOccupancy(char * message, struct Sensor *);
int decodeMessageLight(char * message, struct Sensor *);

/* Fonctions de retrait de donnees des trames en parametre */
int getTemp(char* message);
int getLightLittleSensor(char* message);
int getLightBigSensor(char* message);
int getSwitch(char* message);
int getOccupancy(char* message);
int getContact(char* message);

#endif /* SENSOR_H_ */
