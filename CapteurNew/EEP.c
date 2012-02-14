#include "Component.h" 
#include "ComponentInterface.h"
#include "EEP.h"
#include "string.h"
#include <stdlib.h>
#include "cJSON.h"
#include <stdio.h>
#include "Utility.h"

#define TAILLE_EEP 8
#define TAILLE_NAME 70 


/*
* Initialise la liste des EEP à partir d'un fichier .txt de configuration contenant les informations des capteurs : EEP et nom 
*
*/
int initializeEEPList(char* fileNameEEP, EEP* EEPList){	
	struct EEP* EEPCurrent;
	char eep[TAILLE_EEP+TAILLE_NAME + 30]; 
	cJSON* root;
	int funct;
	int org;
	int type;
	char c;
	int nbOpenedAccolade = 0;
	/* Ouverture du fichier en lecture */
	FILE *f = fopen(fileNameEEP, "r");
	
	EEPCurrent = EEPList;
	/* Pour chaque "capteur" dans le fichier */
	c=fgetc(f);
	while (c!=EOF){
		memset(eep, 0, sizeof (eep));
		/* Récupération du message entier */
		/* Lecture jusqu'au début de l'objet json :*/
		while (c != '{') {
			c=fgetc(f);
		}
		sprintf(eep, "%s%c", eep, c);
		
		/* Lecture de l'objet JSON : */
		nbOpenedAccolade = 1;
		while (nbOpenedAccolade > 0) {
			c=fgetc(f);
			sprintf(eep, "%s%c", eep, c);	
			if (c=='{') {
				nbOpenedAccolade++;
			}
			else if (c=='}') {
				nbOpenedAccolade--;
			}
		}		
		/* Récupération des données EEP et name*/
		root = cJSON_Parse(eep);
		strcpy(EEPCurrent->eep,cJSON_GetObjectItem(root,"EEP")->valuestring);
		strcpy(EEPCurrent->name,cJSON_GetObjectItem(root,"Name")->valuestring);

		
		/* Création des données org, funct, et type */
		org = xtoi(str_sub(EEPCurrent->eep,0,1));
		funct = xtoi(str_sub(EEPCurrent->eep,2,3));
		type = xtoi(str_sub(EEPCurrent->eep,4,5));
		/* Récupération de la fonction et des arguments associés */
		switch(org){
			case 5:{
				switch(funct){
					case 2:{
						EEPCurrent->AddSensors = AddSensorsSwitch;
						break;
					}
					case 3:
					case 4:
					case 10:{
						EEPCurrent->AddSensors =NULL;
						break;
					}
				
				} /* Fin switch funct */
				break;
			} /* Fin case 5 */
			case 6:{
				EEPCurrent->AddSensors = AddSensorsContact;	
				break;
			} /* Fin case 6 */
			case 7:{
				switch(funct){
					case 2:{						
						EEPCurrent->AddSensors =AddSensorsTemp;	
						switch(type){
							case 1:{
								EEPCurrent->scaleMin = -40;
								EEPCurrent->scaleMax = 0;
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 2:{
								EEPCurrent->scaleMin = -30;
								EEPCurrent->scaleMax = 10;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 3:{
								EEPCurrent->scaleMin = -20;
								EEPCurrent->scaleMax = 20;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 4:{
								EEPCurrent->scaleMin = -10;
								EEPCurrent->scaleMax = 30;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 5:{
								EEPCurrent->scaleMin = 0;
								EEPCurrent->scaleMax = 40;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 6:{
								EEPCurrent->scaleMin = 10;
								EEPCurrent->scaleMax = 50;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 7:{
								EEPCurrent->scaleMin = 20;
								EEPCurrent->scaleMax = 60;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 8:{
								EEPCurrent->scaleMin = 30;
								EEPCurrent->scaleMax = 70;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 9:{
								EEPCurrent->scaleMin = 40;
								EEPCurrent->scaleMax = 80;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 16:{
								EEPCurrent->scaleMin = -60;
								EEPCurrent->scaleMax = 20;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;	
								break;
							}
							case 17:{
								EEPCurrent->scaleMin = -50;
								EEPCurrent->scaleMax = 30;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 18:{
								EEPCurrent->scaleMin = -40;
								EEPCurrent->scaleMax = 40;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;	
								break;
							}
							case 19:{
								EEPCurrent->scaleMin = -30;
								EEPCurrent->scaleMax = 50;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 20:{
								EEPCurrent->scaleMin = -20;
								EEPCurrent->scaleMax = 60;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 21:{
								EEPCurrent->scaleMin = -10;
								EEPCurrent->scaleMax = 70;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 22:{
								EEPCurrent->scaleMin = 0;
								EEPCurrent->scaleMax = 80;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 23:{
								EEPCurrent->scaleMin = 10;
								EEPCurrent->scaleMax = 90;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 24:{
								EEPCurrent->scaleMin = 20;
								EEPCurrent->scaleMax = 100;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 25:{
								EEPCurrent->scaleMin = 30;
								EEPCurrent->scaleMax = 110;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 10:{ /* 0A */
								EEPCurrent->scaleMin = 50;
								EEPCurrent->scaleMax = 90;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;	
								break;
							}
							case 11:{ /* 0B */
								EEPCurrent->scaleMin = 60;
								EEPCurrent->scaleMax = 100;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}
							case 26:{ /* 1A */
								EEPCurrent->scaleMin = 40;
								EEPCurrent->scaleMax = 120;	
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;			
								break;
							}
							case 27:{ /*1B */
								EEPCurrent->scaleMin = 50;
								EEPCurrent->scaleMax = 130;		
								EEPCurrent->rangeMin = 255;
								EEPCurrent->rangeMax = 0;		
								break;
							}										
						} /* Fin switch type */
						break;
					} /* Fin case 2 */
					case 8 :{
						EEPCurrent->AddSensors =AddSensorsLightOccupancy;	
						switch(type){
							case 1 :
								EEPCurrent->scaleMin = 0;
								EEPCurrent->scaleMax = 510;
								EEPCurrent->rangeMin = 0;
								EEPCurrent->rangeMax = 255;
								break;
							case 2 :
								EEPCurrent->scaleMin = 0;
								EEPCurrent->scaleMax = 1020;
								EEPCurrent->rangeMin = 0;
								EEPCurrent->rangeMax = 255;
								break;
							case 3 :
								EEPCurrent->scaleMin = 0;
								EEPCurrent->scaleMax = 1530;
								EEPCurrent->rangeMin = 0;
								EEPCurrent->rangeMax = 255;
								break;
						}
						break;
					}
					default:{
						EEPCurrent->AddSensors =NULL;	
						break;
					} /* Fin default */
				} /* Fin switch funct*/
			break;
			} /* Fin case 7 */
		} /* Fin switch org */
		c=fgetc(f);
		if(c!=EOF){
			EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
			EEPCurrent = EEPCurrent->next;
		}

	} /* Fin while */
	
	fclose(f);

	return 0;
}

/* EEPList has to be initialized before calling this function */
int initializeEEPListOld(EEP* EEPList)
{	
	struct EEP* EEPCurrent;
	EEPCurrent = EEPList;

	/*
	**
	**	Org 05
	**
	*/

	/* Insert EEP 05-02-01 */
	strcpy(EEPCurrent->eep,"050201");
	strcpy(EEPCurrent->name,"Interrupteur à 2 boutons, installe a l etat 0\0");
	EEPCurrent->AddSensors = AddSensorsSwitch;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 05-02-02 */
	strcpy(EEPCurrent->eep,"050202");
	strcpy(EEPCurrent->name,"Interrupteur à 2 boutons, installe a l etat 1\0");
	EEPCurrent->AddSensors = AddSensorsSwitch;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));

	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 05-03-01 */
	strcpy(EEPCurrent->eep,"050301");
	strcpy(EEPCurrent->name,"Interrupteur à 4 boutons, installe a l etat 0\0");
	EEPCurrent->AddSensors = NULL;
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));

	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 05-04-01 */
	strcpy(EEPCurrent->eep,"050401");
	strcpy(EEPCurrent->name,"Interrupteur a insertion de carte\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));

	EEPCurrent = EEPCurrent->next;			 

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 05-10-00 */
	strcpy(EEPCurrent->eep,"051000");
	strcpy(EEPCurrent->name,"Capteur de poignee de fenetre\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));

	EEPCurrent = EEPCurrent->next;	

	/*
	**
	**	Org 06
	**
	*/

	/* Insert EEP 06-00-01 */	
	strcpy(EEPCurrent->eep,"060001");
	strcpy(EEPCurrent->name,"Capteur de contact\0");
	EEPCurrent->AddSensors = AddSensorsContact;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;


	/*
	**
	**	Org 07
	**
	*/
	
	/* Insert EEP 07-02-01 */
	strcpy(EEPCurrent->eep,"070201");
	strcpy(EEPCurrent->name,"Capteur de température, entre -40°C et 0°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -40;
	EEPCurrent->scaleMax = 0;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-02 */
	strcpy(EEPCurrent->eep,"070202");
	strcpy(EEPCurrent->name,"Capteur de température, entre -30°C et 10°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -30;
	EEPCurrent->scaleMax = 10;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;
	
	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-03 */
	strcpy(EEPCurrent->eep,"070203");
	strcpy(EEPCurrent->name,"Capteur de température, entre -20°C et 20°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -20;
	EEPCurrent->scaleMax = 20;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-04 */
	strcpy(EEPCurrent->eep,"070204");
	strcpy(EEPCurrent->name,"Capteur de température, entre -10°C et 30°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -10;
	EEPCurrent->scaleMax = 30;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-05 */
	strcpy(EEPCurrent->eep,"070205");
	strcpy(EEPCurrent->name,"Capteur de température, entre 0°C et 40°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 0;
	EEPCurrent->scaleMax = 40;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-06 */
	strcpy(EEPCurrent->eep,"070206");
	strcpy(EEPCurrent->name,"Capteur de température, entre 10°C et 50°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 10;
	EEPCurrent->scaleMax = 50;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	


	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-07 */
	strcpy(EEPCurrent->eep,"070207");
	strcpy(EEPCurrent->name,"Capteur de température, entre 20°C et 60°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 20;
	EEPCurrent->scaleMax = 60;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-08 */
	strcpy(EEPCurrent->eep,"070208");
	strcpy(EEPCurrent->name,"Capteur de température, entre 30°C et 70°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 30;
	EEPCurrent->scaleMax = 70;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	


	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-09 */
	strcpy(EEPCurrent->eep,"070209");
	strcpy(EEPCurrent->name,"Capteur de température, entre 40°C et 80°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 40;
	EEPCurrent->scaleMax = 80;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-0A */
	strcpy(EEPCurrent->eep,"07020A");
	strcpy(EEPCurrent->name,"Capteur de température, entre 50°C et 90°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 50;
	EEPCurrent->scaleMax = 90;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-0B */
	strcpy(EEPCurrent->eep,"07020B");
	strcpy(EEPCurrent->name,"Capteur de température, entre 60°C et 100°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 60;
	EEPCurrent->scaleMax = 100;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-10 */
	strcpy(EEPCurrent->eep,"070210");
	strcpy(EEPCurrent->name,"Capteur de température, entre -60°C et 20°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -60;
	EEPCurrent->scaleMax = 20;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-11 */
	strcpy(EEPCurrent->eep,"070211");
	strcpy(EEPCurrent->name,"Capteur de température, entre -50°C et 30°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -50;
	EEPCurrent->scaleMax = 30;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-12 */
	strcpy(EEPCurrent->eep,"070212");
	strcpy(EEPCurrent->name,"Capteur de température, entre -40°C et 40°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -40;
	EEPCurrent->scaleMax = 40;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-13 */
	strcpy(EEPCurrent->eep,"070213");
	strcpy(EEPCurrent->name,"Capteur de température, entre -30°C et 50°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -30;
	EEPCurrent->scaleMax = 50;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-14 */
	strcpy(EEPCurrent->eep,"070214");
	strcpy(EEPCurrent->name,"Capteur de température, entre -20°C et 60°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -20;
	EEPCurrent->scaleMax = 60;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-15 */
	strcpy(EEPCurrent->eep,"070215");
	strcpy(EEPCurrent->name,"Capteur de température, entre -10°C et 70°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = -10;
	EEPCurrent->scaleMax = 70;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-16 */
	strcpy(EEPCurrent->eep,"070216");
	strcpy(EEPCurrent->name,"Capteur de température, entre 0°C et 80°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 0;
	EEPCurrent->scaleMax = 80;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-17 */
	strcpy(EEPCurrent->eep,"070217");
	strcpy(EEPCurrent->name,"Capteur de température, entre 10°C et 90°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 10;
	EEPCurrent->scaleMax = 90;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-18 */
	strcpy(EEPCurrent->eep,"070215");
	strcpy(EEPCurrent->name,"Capteur de température, entre 20°C et 100°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 20;
	EEPCurrent->scaleMax = 100;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-19*/
	strcpy(EEPCurrent->eep,"070219");
	strcpy(EEPCurrent->name,"Capteur de température, entre 30°C et 110°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 30;
	EEPCurrent->scaleMax = 110;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-1A*/
	strcpy(EEPCurrent->eep,"07021A");
	strcpy(EEPCurrent->name,"Capteur de température, entre 40°C et 120°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 40;
	EEPCurrent->scaleMax = 120;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-1B*/
	strcpy(EEPCurrent->eep,"07021B");
	strcpy(EEPCurrent->name,"Capteur de température, entre 50°C et 130°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 50;
	EEPCurrent->scaleMax = 130;
	EEPCurrent->rangeMin = 255;
	EEPCurrent->rangeMax = 0;		

	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-04-01*/
	strcpy(EEPCurrent->eep,"070401");
	strcpy(EEPCurrent->name,"Capteur de température et d'humidité [0-40°C] [0-100%]\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-06-01*/
	strcpy(EEPCurrent->eep,"070601");
	strcpy(EEPCurrent->name,"Capteur de luminosité, entre 300 et 60000lx\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-06-02*/
	strcpy(EEPCurrent->eep,"070602");
	strcpy(EEPCurrent->name,"Capteur de luminosité, entre 0 et 1024lx\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-07-01*/
	strcpy(EEPCurrent->eep,"070701");
	strcpy(EEPCurrent->name,"Capteur de présence\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-08-01*/
	strcpy(EEPCurrent->eep,"070801");
	strcpy(EEPCurrent->name,"Capteur de lumière et présence [0-510lx]\0");
	EEPCurrent->AddSensors = AddSensorsLightOccupancy;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->scaleMin = 0;
	EEPCurrent->scaleMax = 510;
	EEPCurrent->rangeMin = 0;
	EEPCurrent->rangeMax = 255;
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-08-02*/
	strcpy(EEPCurrent->eep,"070802");
	strcpy(EEPCurrent->name,"Capteur de lumière, température et présence [0-1020lx][0-51°C]\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-08-03*/
	strcpy(EEPCurrent->eep,"070803");
	strcpy(EEPCurrent->name,"Capteur de lumière, température et présence [0-1530lx][-30-50°C]\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-09-01*/
	strcpy(EEPCurrent->eep,"070901");
	strcpy(EEPCurrent->name,"Capteur de gaz\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-09-04*/
	strcpy(EEPCurrent->eep,"070904");
	strcpy(EEPCurrent->name,"Capteur de CO2\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-09-08*/
	strcpy(EEPCurrent->eep,"070908");
	strcpy(EEPCurrent->name,"Capteur de O2\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = EEPCurrent->next;

	/* !!!!!! NOT SUPPORTED YET !!!!!! */
	/* Insert EEP 07-09-0C*/
	strcpy(EEPCurrent->eep,"07090C");
	strcpy(EEPCurrent->name,"Capteur de propane\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
	EEPCurrent = NULL;

	return 0;
}

int destroyEEPList(EEP* p_EEPList){
	char reponse;
	int retour;
	int ok = 0;
	printf("Etes-vous sur de vouloir supprimer la liste d EEP? Ceux non sauvegardes dans le fichier de configuration ne pourront etre recupere (y/n) :\n");
	while (!ok){
	
		retour = scanf("%c%*[^\n]", &reponse);
		if (reponse!= 'y' && reponse != 'Y' && reponse !='n' && reponse != 'N' ){
		/* erreur de saisie, on vide le flux */
		int c;
		while ( ((c = getchar()) != '\n') && c != EOF);
		
		printf("Saisie invalide.\n");
		printf("Veuillez recommencer :\n");
		}
		else {
		/* reussite de la saisie */
		getchar(); /* on enleve le '\n' restant */
		
		printf("saisie acceptee\n");
		break;  /* sort de la boucle */
		}
	}
	if (reponse == 'y' || reponse == 'Y'){
		printf("Suppression EEP\n");
	}
	return 0;
}

/* Fonction permettant d ajouter un capteur a la liste de capteurs a partir de son EEP 
** L EEP est compose de 6 caracteres, provenant de org-funct-type.
** Renvoie 0 si OK, -1 si l EEP n est pas supporte, et -2 si l EEP correspondant est introuvable.
*/
int AddSensorByEEP(char id[8], Sensor ** pp_sensorList, EEP* EEPList, char org[3], char funct[3], char type[3]){
	char eep[6];

	/* Concatenation de org, funct et type dans eep */
	strcpy(eep,org);
	strcat(eep,funct);
	strcat(eep,type);

	while (EEPList != NULL){
		if (strcmp(EEPList->eep,eep) != 0)	/* Si l EEP courant est different de l EEP du capteur */
		{
			EEPList = EEPList->next;
		}else{
			if (EEPList->AddSensors == NULL){
				return NOT_SUPPORTED;	/* L EEP n est pas encore supporte */
			}
			EEPList->AddSensors(id, pp_sensorList, EEPList->scaleMin, EEPList->scaleMax, EEPList->rangeMin, EEPList->rangeMax);
			return OK;
		}
	}
	return NOT_FOUND;	/* EEP introuvable */
}



int AddSensorsContact(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax){	
	Sensor* p_sensor;
	if (*pp_sensorList == NULL){
		*pp_sensorList = (Sensor*)malloc(sizeof(Sensor));
		(*pp_sensorList)->next = NULL;
		p_sensor = *pp_sensorList;
	}else{
		p_sensor = *pp_sensorList;
		while ( (p_sensor != NULL) && (p_sensor->next != NULL) ){
			p_sensor = p_sensor->next;
		}
		if (p_sensor != NULL){
			p_sensor->next = (Sensor*)malloc(sizeof(Sensor));
			p_sensor = p_sensor->next;
		}else{
			p_sensor = (Sensor*)malloc(sizeof(Sensor));
		}
	}
	strcpy(p_sensor->id,id);
	p_sensor->id[8] = 'e';
	p_sensor->id[9] = 'C';
	p_sensor->value = 0;
	p_sensor->decodeMessage = decodeMessageContact;
	p_sensor->next = NULL;
	return 0;
}

int AddSensorsSwitch(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax){
	Sensor* p_sensor;
	if (*pp_sensorList == NULL){
		*pp_sensorList = (Sensor*)malloc(sizeof(Sensor));
		(*pp_sensorList)->next = NULL;
		p_sensor = *pp_sensorList;
	}else{
		p_sensor = *pp_sensorList;
		while ( (p_sensor != NULL) && (p_sensor->next != NULL) ){
			p_sensor = p_sensor->next;
		}
		if (p_sensor != NULL){
			p_sensor->next = (Sensor*)malloc(sizeof(Sensor));
			p_sensor = p_sensor->next;
		}else{
			p_sensor = (Sensor*)malloc(sizeof(Sensor));
		}
	}
	strcpy(p_sensor->id,id);
	p_sensor->id[8] = 'e';
	p_sensor->id[9] = 'S';
	p_sensor->value = 0;
	p_sensor->decodeMessage = decodeMessageSwitch;
	p_sensor->next = NULL;
	return 0;
}

int AddSensorsTemp(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax){
	Sensor* p_sensor;
	if (*pp_sensorList == NULL){
		*pp_sensorList = (Sensor*)malloc(sizeof(Sensor));
		(*pp_sensorList)->next = NULL;
		p_sensor = *pp_sensorList;
	}else{
		p_sensor = *pp_sensorList;
		while ( (p_sensor != NULL) && (p_sensor->next != NULL) ){
			p_sensor = p_sensor->next;
		}
		if (p_sensor != NULL){
			p_sensor->next = (Sensor*)malloc(sizeof(Sensor));
			p_sensor = p_sensor->next;
		}else{
			p_sensor = (Sensor*)malloc(sizeof(Sensor));
		}
	}
	strcpy(p_sensor->id,id);
	p_sensor->id[8] = 'e';
	p_sensor->id[9] = 'T';
	p_sensor->value = 0;
	p_sensor->rangeData = (Range*)malloc(sizeof(Range));
	p_sensor->rangeData->scaleMax = scaleMax;
	p_sensor->rangeData->scaleMin = scaleMin;
	p_sensor->rangeData->rangeMax = rangeMax;
	p_sensor->rangeData->rangeMin = rangeMin;	
	p_sensor->decodeMessage = decodeMessageTemp;
	p_sensor->next = NULL;
	return 0;	
}

int AddSensorsLightOccupancy(char id[8], Sensor ** pp_sensorList, float scaleMin, float scaleMax, float rangeMin, float rangeMax){
	Sensor* p_sensor;
	if (*pp_sensorList == NULL){
		*pp_sensorList = (Sensor*)malloc(sizeof(Sensor));
		(*pp_sensorList)->next = NULL;
		p_sensor = *pp_sensorList;
	}else{		
		p_sensor = *pp_sensorList;
		while ( (p_sensor != NULL) && (p_sensor->next != NULL) ){
			p_sensor = p_sensor->next;
		}
		if (p_sensor != NULL){
			p_sensor->next = (Sensor*)malloc(sizeof(Sensor));
			p_sensor = p_sensor->next;
		}else{
			p_sensor = (Sensor*)malloc(sizeof(Sensor));
		}
	}	
	strcpy(p_sensor->id,id);
	p_sensor->id[8] = 'e';
	p_sensor->id[9] = 'L';
	p_sensor->value = 0;
	p_sensor->rangeData = (Range*)malloc(sizeof(Range));
	p_sensor->rangeData->scaleMax = scaleMax;
	p_sensor->rangeData->scaleMin = scaleMin;
	p_sensor->rangeData->rangeMax = rangeMax;
	p_sensor->rangeData->rangeMin = rangeMin;
	p_sensor->decodeMessage = decodeMessageLight;

	p_sensor->next = (Sensor*)malloc(sizeof(Sensor));
	p_sensor = p_sensor->next;

	strcpy(p_sensor->id,id);
	p_sensor->id[8] = 'e';
	p_sensor->id[9] = 'O';
	p_sensor->value = 0;	
	p_sensor->decodeMessage = decodeMessageOccupancy;
	p_sensor->next = NULL;

	return 0;
}
