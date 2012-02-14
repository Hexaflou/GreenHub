/*
* Ce fichier permet de cr�er une configuration de base : cr�er le fichier .txt qui contient les informations sur les capteurs
*
*
*/

#include "Component.h"
#include "ComponentInterface.h"
#include "EEP.h"
#include "cJSON.h"
#include <stdio.h>

/*
* Retourne le cJSON contenant les donn�es pass�es en param�tre : l'id et l'EEP
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
* Cr�e le fichier .txt contenant les informations sur les capteurs
*
*/
void createConfigSensors(){
	char* sensor;
	cJSON* jsonSensor;
	
	/* Ouverture du fichier en �criture */
	FILE *f = fopen("sensors.txt", "w");  
	
	/* Cr�ation et �criture des cJSON */
  	jsonSensor= createCSON("00893378", "070205");
	sensor = cJSON_Print(jsonSsensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, sensor);  
	
   	jsonSensor= createCSON("0021CBE5", "050201");
	sensor = cJSON_Print(jsonSsensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, sensor);  
	
  	jsonSensor= createCSON("0001B015", "060001");
	sensor = cJSON_Print(jsonSsensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, sensor);  
	
  	jsonSensor= createCSON("0001B016", "060001");
	sensor = cJSON_Print(jsonSsensor);
	cJSON_Delete(jsonSensor);
    fprintf(f, sensor);  
  
	/* Fermeture du fichier */
    fclose(f);  
}