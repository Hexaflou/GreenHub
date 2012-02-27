#ifndef COMPONENTINTERFACE_H_
#define COMPONENTINTERFACE_H_

/***********************************************************************
 *			InterfaceComposant.h
 * 
 * Interface utilisateur permettant d'utiliser les capteurs/actionneurs
 * EnOcean, ainsi que les capteurs SunSpot.
 *
 * 							Hexanome H4212
 * ********************************************************************/


/* Définition de macros */
#define SOCKET_ERROR -1
#define ERROR -1
#define COMPONENT_ALREADY_EXIST -3
#define INVALID_ID -2
#define OK 0
#define DEBUG 0
#define MAX_MQ_SIZE 1024

/* Inclusions externes */
#include <semaphore.h>

/* Inclusions internes */
#include "Utility.h"
#include "Sensor.h"
#include "Actuator.h"
#include "EEP.h"

/*
 * Fonction d'initialisation pour les composants utilisés par l'interface.
 * Renvoie 0 si OK, -1 si l'initialisation a déjà été faite.
 */
int ComponentInterfaceInit();

/*
 * Fonction de destruction des composantes utilisés par l'interface.
 */
int ComponentInterfaceClose();

/*
 * Fonction de traitement utilisé par les tâches d'écoute SunSpot et EnOcean
 * pour retrouver le capteur correspondant au message.
 * Entrée : Chapine de caractère message sans le header (A55A+SYNC_BITE+LENGTH)
 */
void ManageMessage(char* message);


/*
 * Permet de récupérer à travers “value” la valeur d’un capteur. La mesure (température, light, …)
 * n’est pas explicitement donnée, elle doit être connue en amont ou être déduite de l’ID.
 * Renvoie un int correspondant au résultat de la fonction :
 * -1 (ERROR) correspond à une erreur (périphérique absent, …) avec value = 0, et 0 si tout s’est bien passé..
 */
int GetInfoFromSensor(char id[10], float * p_value);

/*
 * Fonction permettant d’ajouter un capteur. Un capteur est identifié par son ID (en 8 caractères),
 * et son type caractérisé par son EEP.
 * Exemple : 07-01-01 : Org = 7; Function = 01; Type = 01, correspondant à un capteur de présence.
 * Renvoie 0 si tout s’est bien déroulé, ou -1 s’il y a eu une erreur car l’EEP n’est pas supporté,
 * -2 si l’EEP est introuvable et -3 si l’ID est déjà présent dans la liste.
 */
int AddComponent(char * id, char org[2], char funct[2], char type[2]);

/*
 * Consulte l'état de l'actionneur dont l'ID est donné en paramètre.
 * L'état est renvoyé par l'intermédiaire du paramètre p_value.
 */
int GetStatusFromActuator(char *id, float * p_value);

/*
 * Cette fonction va chercher dans notre liste d’actionneurs le bon, pour lancer l’action.
 * L’ID ici doit être de 10 caractères comme il a été convenu + ‘\0’.
 * “value” représente la valeur à faire passer à l’actionneur.
 */
int ActionActuator(char *id, float value);

/*
 * Renvoie la liste de capteurs.
 */
Sensor * getSensorList();

/*
 * Renvoie la liste d'actionneurs.
 */
Actuator * getActuatorList();

/*
 * Renvoie le sémaphore mutex protégeant la liste de capteurs.
 */
sem_t getSemSensor();

/*
 * Renvoie le sémaphore mutex protégeant la liste d'actionneurs.
 */
sem_t getSemActuator();

#endif /* COMPONENTINTERFACE_H_ */
