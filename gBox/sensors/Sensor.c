/*
 * Sensor.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Sensor.h"
#include "Utility.h"
#include "../gLogs.h"
#include "ComponentInterface.h"



/*******************************************************
 *
 *		FONCTIONS DE DECODAGE
 *
 ******************************************************/

/*
**	Fonction de decodage d une mesure lumineuse (en lx)
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageLight(char * message, struct Sensor* p_sensor){
	float light;
	float multiplier;

	light = (float) getLightLittleSensor(message);	/* Valeur entre rangeMin et rangeMax (en general 0..255) */

	/* Calcul du multiplicateur pour determiner la vraie valeur mesuree par le capteur */
	multiplier = ( (((SensorRange*) p_sensor->rangeData)->scaleMax - ((SensorRange*) p_sensor->rangeData)->scaleMin)
		/ (((SensorRange*) p_sensor->rangeData)->rangeMax - ((SensorRange*) p_sensor->rangeData)->rangeMin));
	if (multiplier < 0){
		light = light * multiplier + ((SensorRange*) p_sensor->rangeData)->scaleMax;
	}else{
		light = light * multiplier + ((SensorRange*) p_sensor->rangeData)->scaleMin;
	}

	/* Si la nouvelle valeur est differente de l ancienne */
	if (p_sensor->value != light)
	{
		p_sensor->value = light;
		gLogsLog (p_sensor->id, p_sensor->value);
		printf("Valeur du capteur de luminosite : %f \n", p_sensor->value);
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d une mesure de presence
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageOccupancy(char* message, struct Sensor* p_sensor)
{
	int occupancy;
	occupancy = getOccupancy(message);
	printf("Mesure de presence : %i \n", occupancy);
	if (occupancy == 1){
		printf("Aucune presence detectee. \n");
	}else{
		printf("Presence detectee. \n");
	}
	/* Si la nouvelle valeur est differente de l ancienne */
	if (occupancy != p_sensor->value){
		p_sensor->value = occupancy;
		gLogsLog (p_sensor->id, p_sensor->value);
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d une mesure de temperature (en °C)
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageTemp(char* message, struct Sensor* p_sensor)
{
	float temp, multiplier;
	temp = (float) getTemp(message);	/* Valeur entre rangeMin et rangeMax (en general 0..255) */

	/* Calcul du multiplicateur pour determiner la vraie valeur mesuree par le capteur */
	multiplier = ( (((SensorRange*) p_sensor->rangeData)->scaleMax - ((SensorRange*) p_sensor->rangeData)->scaleMin)
		/ (((SensorRange*) p_sensor->rangeData)->rangeMax - ((SensorRange*) p_sensor->rangeData)->rangeMin));
	if (multiplier < 0){
		temp = temp * multiplier + ((SensorRange*) p_sensor->rangeData)->scaleMax;
	}else{
		temp = temp * multiplier + ((SensorRange*) p_sensor->rangeData)->scaleMin;
	}
	printf("Valeur du capteur de temperature : %f \n", temp);

	/* Si la nouvelle valeur est differente de l ancienne */
	if (p_sensor->value != temp)
	{
		p_sensor->value = temp;
		gLogsLog (p_sensor->id, p_sensor->value);
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}

/*
**	Fonction de decodage d un message de detection de contact
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageContact(char* message, struct Sensor * p_sensor)
{
	int closed;
	closed = getContact(message);


	if (closed == 1){
		printf("Contact ferme. \n");
		/*ActionActuator("0021CBE5aC00\0", B0);*/
	}else{
		printf("Contact ouvert. \n");
		/*ActionActuator("0021CBE5aC00\0", B1);*/
	}
	/* Si la nouvelle valeur est differente de l ancienne */
	if (closed != p_sensor->value){
		p_sensor->value = closed;
		gLogsLog (p_sensor->id, p_sensor->value);
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d un appui sur un interrupteur
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageSwitch(char* message, struct Sensor * p_sensor)
{
	int switch_button = getSwitch(message);
/*	if (switch_button == A0){
		ActionActuator("0021CBE5aC00\0", B0);
	}
	if (switch_button == A1){
		ActionActuator("0021CBE5aC00\0", B1);
	}*/
	if (switch_button != NO_BUTTON){
		printf("Valeur de l interrupteur : %i \n", switch_button);
		/* Si la nouvelle valeur est differente de l ancienne */
		if (switch_button != p_sensor->value)
		{
			p_sensor->value = switch_button;
			gLogsLog (p_sensor->id, p_sensor->value);
			return VALUE_CHANGE;
		}
		else
		{
			return NO_CHANGE;
		}
	}
	return NO_BUTTON;
}


/*******************************************************
 *
 *	FONCTIONS DE TRAITEMENT DE MESSAGE
 *
 ******************************************************/

/*
 * Retourne la temperature donnee par le message en parametre.
 * Le message doit correspondre a un message envoye par un capteur de temperature, auquel cas le message ne sera pas pertinent.
 */
int getTemp(char* message)
{
	int temp;
	temp = xtoi(str_sub(message, 6, 7)); /* Extraction de la temperature a partir du message */
	return temp;
}

/*
 * Retourne la luminosite donnee par le message en parametre. Celle-ci est exprimee sur un octet.
 * Le message doit correspondre a un message envoye par un capteur de luminosite, auquel cas le message ne sera pas pertinent.
 */
int getLightLittleSensor(char* message)
{
	int light;
	light = xtoi(str_sub(message, 4, 5)); /* Extraction de la luminosite a partir du message */
	return light;
}

/*
 * Retourne la temperature donnee par le message en parametre. Celle-ci est exprimee sur deux octets.
 * Le message doit correspondre a un message envoye par un capteur de luminosite, auquel cas le message ne sera pas pertinent.
 */
int getLightBigSensor(char* message)
{
	int light;
	light = xtoi(str_sub(message, 4, 7)); /* Extraction de la luminosite a partir du message */
	return light;
}

/*
 * Retourne la mesure de presence donnee par le message en parametre.
 * Le message doit correspondre a un message envoye par un capteur de presence, auquel cas le message ne sera pas pertinent.
 */
int getOccupancy(char* message)
{
	int occupancy, byte;
	byte = xtoi(str_sub(message, 8, 9)); /* Extraction de l octet de donnee a partir du message */
	occupancy = (byte & 0x02)>>1; /* Extraction du bit 1 de l octet */
	printf("Resultat : %i\n", occupancy);
	return occupancy;
}

/*
 * Retourne le bouton presse sur un interrupteur par le message en parametre.
 * Le message doit correspondre a un message envoye par un interrupteur, auquel cas le message ne sera pas pertinent.
 */
int getSwitch(char* message)
{
	int result;
	int byte, status;
	byte = xtoi(str_sub(message, 2, 3)); /* Extraction de l octet de donnee a partir du message */
	status = xtoi(str_sub(message, 18, 19)); /* Extraction de l octet de statut a partir du message */
	if (status & (1u << 4))
	{ /* Si le message est de type N */
		result = ((byte & 0xE0) >> 5); /* Extraction du bit 5 à 7 de l octet */
	}
	else
	{ /* Si le message est de type U */
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

/*
 * Retourne la detection de contact par le message en parametre.
 * Le message doit correspondre a un message envoye par un capteur de contact, auquel cas le message ne sera pas pertinent.
 */
int getContact(char* message)
{
	int closed, byte;
	byte = xtoi(str_sub(message, 8, 9)); /* Extraction de l octet de donnee a partir du message */
	closed = byte & 0x01; /* Extraction du bit 0 de l octet */
	return closed;
}
