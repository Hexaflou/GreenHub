/*
 * Ce fichier permet de lire et réécrire la configuration du système à partir d'un fichier .txt
 *
 * 
 */

#include "Test.h"
#include "Component.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "cJSON.h"

#define TAILLE_ID 8
#define TAILLE_EEP 6

 /*
* Retourne le cJSON contenant les données passées en paramètre : l'id et l'EEP
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
* Charge la configuration du système à partir de la lecture d'un fichier contenant l'EEP et l'id des capteurs
*
*/
void readConfig(char* fileName, Sensor * p_sensorList, EEP* EEPList){
	
	char* sensor;
	cJSON *root;
	char org[2];
	char funct[2]; 
	char type[2];
	
	/* Ouverture du fichier en lecture */
	FILE *f = fopen(fileName, "r");  
	
	EEPList = (EEP*)malloc(sizeof(EEP));
	
	/* Initialisaition de la EEPList */
	initializeEEPList(EEPList);
	
	/* Récupération des informations de chaque capteur */
	while (fgets(sensor,TAILLE_ID+TAILLE_EEP, f) != NULL){
		root = cJSON_Parse(sensor);
		org[0]= cJSON_GetObjectItem(root,"EEP")->valuestring[0];
		org[1]= cJSON_GetObjectItem(root,"EEP")->valuestring[1];
		funct[0]=cJSON_GetObjectItem(root,"EEP")->valuestring[2];
		funct[1]=cJSON_GetObjectItem(root,"EEP")->valuestring[3];
		type[0]=cJSON_GetObjectItem(root,"EEP")->valuestring[4];
		type[1]=cJSON_GetObjectItem(root,"EEP")->valuestring[5];
		/* Ajout du capteur */
		AddSensorByEEP(cJSON_GetObjectItem(root,"id")->valuestring, p_sensorList, EEPList, org, funct, type);
		cJSON_Delete(root);
	}

}
/*
* Enregistre la configuration du système (contenant l'EEP et l'id des capteurs) dans un fichier 
*
*/
void rewriteConfig(char* fileName, Sensor * p_sensorList){
	Sensor * pCurrent;
	cJSON * root;
	char * sensor:
	
	/* Ouverture du fichier en écriture */
	FILE *f = fopen(fileName, "w");  
	
	/* Parcours de la liste de capteurs */
	if (p_sensorList!= NULL) {
		pCurrent = p_sensorList;

		while (pCurrent->next != NULL){
			/* Création du json */
			root = createCSON(pCurrent->id,pCurrent->EEP);
			sensor = cJSON_Print(root);
			/* Ecriture des données */
			fprintf(f,sensor);  
			
			cJSON_Delete(root);
			pCurrent = pCurrent->next;
		}
		
	}

	/* Fermeture du fichier */
	fclose(f);

}