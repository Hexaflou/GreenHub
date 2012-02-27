#ifndef EEP_H_
#define EEP_H_

/***********************************************************************
 *			EEP.h
 *
 * Interface permettant d'exploiter les EEP des composants, d'ajouter
 * des composants (capteur/actionneur).
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions internes */
#include "Sensor.h"
#include "Actuator.h"

/* Déclaration de constantes */
#define NOT_SUPPORTED -1
#define NOT_FOUND -2

typedef struct EEP
{
	char eep[7];
	int (*AddComponent)(char* id, void ** pp_ComponentList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
	char name[70];
	struct EEP* next;
	/* Arguments optionnels pour spécifier la portée de la mesure */	
	int scaleMin;
	int scaleMax;	
	int rangeMin;
	int rangeMax;
}EEP;

/*
 * Construit la liste d'EEP à partir d'un fichier de configuration dont le chemin est donné en paramètre
 */
int initializeEEPList(char*,EEP*);

/*
 * Supprime la liste d'EEP
 */
int destroyEEPList(EEP* p_EEPList);

int writeEEPList(char* fileNameEEP, EEP* p_EEPList);

/*
 * Ajoute un composant en fonction de son EEP.
 * Dans le cas où le composant est un actionneur, l'id doit être constitué de 11 caractères sans compter '\0'.
 * Dans le cas où le composant est un capteur, l'id doit être constitué de 8 caractères sans compter '\0'.
 */
int AddComponentByEEP(char * id, void ** pp_componentList, EEP* p_EEPList, char org[3], char funct[3], char type[3]);

/***********************************
 *  Fonctions d'ajout de capteurs
 ***********************************/
int AddSensorContact(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorSwitch(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorTemp(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorLightOccupancy(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddSensorTempLightSunSpot(char * id, void ** pp_sensorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);

/***********************************
 *  Fonctions d'ajout d'actionneurs
 ***********************************/
int AddActuatorCurrent(char * id, void ** pp_ActuatorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);
int AddActuatorTemp(char * id, void ** pp_ActuatorList, char eep[7], float scaleMin, float scaleMax, float rangeMin, float rangeMax);

#endif /* EEP_H_ */
 
