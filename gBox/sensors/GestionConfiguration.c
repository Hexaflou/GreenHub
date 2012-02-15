/*
 * Ce fichier permet de lire et r��crire la configuration du syst�me � partir d'un fichier .txt
 *
 * 
 */

#include "Test.h"
#include "Component.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "../lib/cJSON.h"
#include <stdio.h>

#define TAILLE_ID_SENSOR 8
#define TAILLE_ID_ACTUATOR 13
#define TAILLE_EEP 6

 /*
* Retourne le cJSON contenant les donnees passees en parametre : l'id et l'EEP
*
*/
cJSON* createCSON(char* id, char* EEP){
	cJSON *root ; 
	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"id",id);
	cJSON_AddStringToObject(root,"EEP",EEP);
	return root;
}



/*
* Charge la configuration du systeme a partir de la lecture d'un fichier contenant l'EEP et l'id des capteurs
* 
*
*/
void readConfig(char* fileNameSensor, char* fileNameEEP, char* fileNameActuator, Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList){
	
	char sensor[TAILLE_ID_SENSOR+TAILLE_EEP+40];
	char actuator[TAILLE_ID_ACTUATOR+TAILLE_EEP+40];
	cJSON *root;
	char org[3];
	char funct[3]; 
	char type[3];
	int c;
	char * id;
	char * eepstr = NULL;
	int nbOpenedAccolade;
	
	/* Ouverture du fichier en lecture */
	FILE *f = fopen(fileNameSensor, "r");
	
	if (f == NULL)
	{
		printf("ERREUR dans l'ouverture du fichier de capteurs. \n");
	}
	/* Initialisation de la EEPList */
	initializeEEPList(fileNameEEP, EEPList);
	
	/* Recuperation des informations de chaque capteur */
	c=fgetc(f);
	while (c!=EOF){
		memset (sensor, 0, sizeof (sensor));
		/* Lecture jusqu'au debut de l'objet json :*/
		while (c != '{') {
			c=fgetc(f);
		}
		sprintf(sensor, "%s%c", sensor, c);
		
		/* Lecture de l'objet JSON : */
		nbOpenedAccolade = 1;
		while (nbOpenedAccolade > 0) {
			c=fgetc(f);
			sprintf(sensor, "%s%c", sensor, c);	
			if (c=='{') {
				nbOpenedAccolade++;
			}
			else if (c=='}') {
				nbOpenedAccolade--;
			}
		}	
		sprintf(sensor, "%s%c", sensor, '\0');	
		/* Recuperation des donnees du json */
		root = cJSON_Parse(sensor);
		if (root != NULL){
			/* EEP */
			eepstr= cJSON_GetObjectItem(root,"EEP")->valuestring;
			if(eepstr == NULL)
				printf("error\n");
			/* org, funct, type */
			strncpy(org,str_sub(eepstr,0,1), 3);
			strncpy(funct,str_sub(eepstr,2,3), 3);
			strncpy(type,str_sub(eepstr,4,5), 3);
			/* id */
			id=cJSON_GetObjectItem(root,"id")->valuestring;
			if(id == NULL)
				printf("error 2\n");
			/* Ajout du capteur */
			AddComponentByEEP(id, (void**) pp_sensorList, EEPList, org, funct, type);
			cJSON_Delete(root);
		}
		c=fgetc(f);
	} /* Fin while */
	/* Fermeture du fichier */
	fclose(f);
	
	/* Ouverture du fichier d actionneur */
	f = fopen(fileNameActuator, "r");
	if (f == NULL)
	{
		printf("ERREUR dans l'ouverture du fichier d actionneurs.\n");
	}
	
	/* Recuperation des informations de chaque actionneur */
	c=fgetc(f);
	while (c!=EOF){
		memset (actuator, 0, sizeof (actuator));
		/* Lecture jusqu'au debut de l'objet json :*/
		while (c != '{') {
			c=fgetc(f);
		}
		sprintf(actuator, "%s%c", actuator, c);
		
		/* Lecture de l'objet JSON : */
		nbOpenedAccolade = 1;
		while (nbOpenedAccolade > 0) {
			c=fgetc(f);
			sprintf(actuator, "%s%c", actuator, c);	
			if (c=='{') {
				nbOpenedAccolade++;
			}
			else if (c=='}') {
				nbOpenedAccolade--;
			}
		}	
		sprintf(actuator, "%s%c", actuator, '\0');	
		/* Recuperation des donnees du json */
		root = cJSON_Parse(actuator);
		if (root != NULL){
			/* EEP */
			eepstr= cJSON_GetObjectItem(root,"EEP")->valuestring;
			if(eepstr == NULL)
				printf("error\n");
			/* org, funct, type */
			strncpy(org,str_sub(eepstr,0,1), 3);
			strncpy(funct,str_sub(eepstr,2,3), 3);
			strncpy(type,str_sub(eepstr,4,5), 3);
			/* id */
			id=cJSON_GetObjectItem(root,"id")->valuestring;
			if(id == NULL)
				printf("error 2\n");
			/* Ajout du capteur */			
			AddComponentByEEP(id, (void**) pp_actuatorList, EEPList, org, funct, type);
			cJSON_Delete(root);
		}
		c=fgetc(f);
	} /* Fin while */
	/* Fermeture du fichier */
	fclose(f);	
}

/*
** Enregistre la configuration du systeme : la liste des eep et la liste des capteurs
** 
*/
void writeConfig(char* fileNameSensor,char* fileNameEEP, Sensor * p_sensorList,EEP* p_EEPList){
	Sensor * pCurrent;
	cJSON * root;
	char * sensor;
	FILE *f;
	
	/* Ecriture du fichier eep */
	writeEEPList(fileNameEEP,p_EEPList);


	/* Ouverture du fichier en ecriture */
	f = fopen(fileNameSensor, "w");
	
	
	if (p_sensorList!= NULL) {
		pCurrent = p_sensorList;
		/* Parcours de la liste de capteurs */
		while (pCurrent!= NULL){
			/* Creation du json */
			root = createCSON(str_sub(pCurrent->id,0,7),str_sub(pCurrent->EEP,0,5));
			sensor = cJSON_Print(root);
			/* Ecriture des donnees */
			fprintf(f,"%s",sensor);  
			
			cJSON_Delete(root);
			pCurrent = pCurrent->next;
		}
		
	}

	/* Fermeture du fichier */
	fclose(f);

}
