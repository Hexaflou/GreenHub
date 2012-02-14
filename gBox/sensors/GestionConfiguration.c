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

#define TAILLE_ID 8
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
void readConfig(char* fileNameSensor, char* fileNameEEP, Sensor ** pp_sensorList, EEP* EEPList){
	
	char sensor[TAILLE_ID+TAILLE_EEP+40];
	cJSON *root;
	char org[3];
	char funct[3]; 
	char type[3];
	char c;
	char * eepstr = NULL;
	int nbOpenedAccolade;
	
	/* Ouverture du fichier en lecture */
	FILE *f = fopen(fileNameSensor, "r");
	
	
	/* Initialisation de la EEPList */
	initializeEEPList(fileNameEEP, EEPList);
	
	/* Recuperation des informations de chaque capteur */
	c=fgetc(f);
	while (c!=EOF){
		memset (sensor, 0, sizeof (sensor));
		/* Recuperation du message entier */
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
		root = cJSON_Parse(sensor);
		if (root != NULL){
			eepstr= cJSON_GetObjectItem(root,"EEP")->valuestring;
			if(eepstr == NULL)
				printf("error\n");
			strncpy(org,str_sub(eepstr,2,3), 3);
			strncpy(funct,str_sub(eepstr,2,3), 3);
			strncpy(type,str_sub(eepstr,4,5), 3);
			/* Ajout du capteur */
			eepstr=cJSON_GetObjectItem(root,"id")->valuestring;
			if(eepstr == NULL)
				printf("error 2\n");
			AddSensorByEEP(eepstr, pp_sensorList, EEPList, org, funct, type);
			/*cJSON_Delete(root);*/
		}
		c=fgetc(f);
	}

}
/*
* Enregistre la configuration du systeme (contenant l'EEP et l'id des capteurs) dans un fichier 
* ATTENTION PAS FINI !
*/
void rewriteConfig(char* fileName, Sensor * p_sensorList){
	Sensor * pCurrent;
	cJSON * root;
	char * sensor;
	
	/* Ouverture du fichier en �criture */
	FILE *f = fopen(fileName, "w");  
	
	/* Parcours de la liste de capteurs */
	if (p_sensorList!= NULL) {
		pCurrent = p_sensorList;

		while (pCurrent->next != NULL){
			/* Cr�ation du json */
			root = createCSON(pCurrent->id,pCurrent->EEP);
			sensor = cJSON_Print(root);
			/* Ecriture des donn�es */
			fprintf(f,sensor);  
			
			cJSON_Delete(root);
			pCurrent = pCurrent->next;
		}
		
	}

	/* Fermeture du fichier */
	fclose(f);

}
