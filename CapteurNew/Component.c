/*
 * Component.c
 *
 *  Created on: 19 janv. 2012
 *      Author: nikuya
 */

#include "Component.h"
#include "Utility.h"
#include "gLogs.h"


/*
**	Fonction de decodage d une mesure lumineuse (en lx)
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageLight(char * message, struct Sensor sensor){
	float light;
	float multiplier;
	light = (float) getLightLittleSensor(message);	/* Valeur entre rangeMin et rangeMax (en general 0..255) */

	/* Calcul du multiplicateur pour determiner la vraie valeur mesuree par le capteur */
	multiplier = ( (((Range*) sensor.rangeData)->scaleMax - ((Range*) sensor.rangeData)->scaleMin)
		/ (((Range*) sensor.rangeData)->rangeMax - ((Range*) sensor.rangeData)->rangeMin));
	if (multiplier < 0){
		light = light * multiplier + ((Range*) sensor.rangeData)->scaleMax;	
	}else{
		light = light * multiplier + ((Range*) sensor.rangeData)->scaleMin;
	}

	/* Si la nouvelle valeur est differente de l ancienne */
	if (sensor.value != light)
	{
		sensor.value = light;
		printf("Valeur du capteur de luminosite : %f \n", sensor.value);
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d une mesure de presence
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageOccupancy(char* message, struct Sensor sensor)
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
	if (occupancy != sensor.value){
		/*gLogLogs... */
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d une mesure de temperature (en °C)
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageTemp(char* message, struct Sensor sensor)
{
	float temp, multiplier;
	temp = (float) getTempWithoutRange(message);	/* Valeur entre rangeMin et rangeMax (en general 0..255) */

	/* Calcul du multiplicateur pour determiner la vraie valeur mesuree par le capteur */
	multiplier = ( (((Range*) sensor.rangeData)->scaleMax - ((Range*) sensor.rangeData)->scaleMin)
		/ (((Range*) sensor.rangeData)->rangeMax - ((Range*) sensor.rangeData)->rangeMin));
	if (multiplier < 0){
		temp = temp * multiplier + ((Range*) sensor.rangeData)->scaleMax;	
	}else{
		temp = temp * multiplier + ((Range*) sensor.rangeData)->scaleMin;
	}

	printf("Valeur du capteur de temperature : %f \n", sensor.value);

	/* Si la nouvelle valeur est differente de l ancienne */
	if (sensor.value != temp)
	{
		sensor.value = temp;				
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}

/*
**	Fonction de decodage d un message de detection de contact
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageContact(char* message, struct Sensor sensor)
{
	int closed;
	closed = getContact(message);
	if (closed == 1){
		printf("Contact ferme. \n");
	}else{
		printf("Contact ouvert. \n");
	}
	/* Si la nouvelle valeur est differente de l ancienne */
	if (closed != sensor.value){
		sensor.value = closed;
		return VALUE_CHANGE;
	}
	return NO_CHANGE;
}


/*
**	Fonction de decodage d un appui sur un interrupteur
**	Si la nouvelle valeur est differente de l ancienne, elle est mise a jour, et celle-ci est reporte dans un fichier de log.
**	Renvoie 0 si un changement de valeur a eu lieu, 1 sinon.
*/
int decodeMessageSwitch(char* message, struct Sensor sensor)
{
	int switch_button = getSwitch(message);	
	if (switch_button != NO_BUTTON){
		printf("Valeur de l interrupteur : %i \n", switch_button);
		/* Si la nouvelle valeur est differente de l ancienne */
		if (switch_button != sensor.value)
		{		
			sensor.value = switch_button;
			return VALUE_CHANGE;
		}
		else
		{
			return NO_CHANGE;
		}
	}
}


/*
 * Retourne la temperature donnee par le message en parametre.
 * Le message doit correspondre a un message envoye par un capteur de temperature, auquel cas le message ne sera pas pertinent.
 */
int getTempWithoutRange(char* message)
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
