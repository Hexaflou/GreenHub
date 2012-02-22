/*
* Ce fichier permet de créer une configuration de base : créer le fichier .txt qui contient les informations sur les capteurs
*
*
*/

#include "Component.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "../lib/cJSON.h"
#include <stdio.h>

/*
* Retourne le cJSON contenant les données passées en paramètre : l'id et l'EEP
*
*/
cJSON* createCSONFirst(char* id, char* EEP){
	cJSON *root ; 
	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"id",id);
	cJSON_AddStringToObject(root,"EEP",EEP);
	return root;
}
 
/*
* Crée le fichier .txt contenant les informations sur les capteurs
*
*/
void createConfigSensors(){
	char* sensor;
	cJSON* jsonSensor;
	
	/* Ouverture du fichier en écriture */
	FILE *f = fopen("sensors.txt", "w");  
	
	/* Création et écriture des cJSON */
  	jsonSensor= createCSONFirst("00893378", "070205");
	sensor = cJSON_Print(jsonSensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, "%s",sensor);  
	
   	jsonSensor= createCSONFirst("0021CBE5", "050201");
	sensor = cJSON_Print(jsonSensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, "%s",sensor);  
	
  	jsonSensor= createCSONFirst("0001B015", "060001");
	sensor = cJSON_Print(jsonSensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, "%s",sensor);  
	
  	jsonSensor= createCSONFirst("0001B016", "060001");
	sensor = cJSON_Print(jsonSensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, "%s",sensor);  
  
	/* Fermeture du fichier */
    fclose(f);  
}
