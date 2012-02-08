#include "Component.h" 
#include "EEP.h"
#include "string.h"
#include <stdlib.h>

/* EEPList has to be initialized before calling this function */
int initializeEEPList(EEP* EEPList)
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
	EEPCurrent->arg1 = -40;
	EEPCurrent->arg2 = 0;	

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-02 */
	strcpy(EEPCurrent->eep,"070202");
	strcpy(EEPCurrent->name,"Capteur de température, entre -30°C et 10°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -30;
	EEPCurrent->arg2 = 10;	
	
	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-03 */
	strcpy(EEPCurrent->eep,"070203");
	strcpy(EEPCurrent->name,"Capteur de température, entre -20°C et 20°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -20;
	EEPCurrent->arg2 = 20;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-04 */
	strcpy(EEPCurrent->eep,"070204");
	strcpy(EEPCurrent->name,"Capteur de température, entre -10°C et 30°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -10;
	EEPCurrent->arg2 = 30;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-05 */
	strcpy(EEPCurrent->eep,"070205");
	strcpy(EEPCurrent->name,"Capteur de température, entre 0°C et 40°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 0;
	EEPCurrent->arg2 = 40;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-06 */
	strcpy(EEPCurrent->eep,"070206");
	strcpy(EEPCurrent->name,"Capteur de température, entre 10°C et 50°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 10;
	EEPCurrent->arg2 = 50;		


	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-07 */
	strcpy(EEPCurrent->eep,"070207");
	strcpy(EEPCurrent->name,"Capteur de température, entre 20°C et 60°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 20;
	EEPCurrent->arg2 = 60;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-08 */
	strcpy(EEPCurrent->eep,"070208");
	strcpy(EEPCurrent->name,"Capteur de température, entre 30°C et 70°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 30;
	EEPCurrent->arg2 = 70;		


	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-09 */
	strcpy(EEPCurrent->eep,"070209");
	strcpy(EEPCurrent->name,"Capteur de température, entre 40°C et 80°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 40;
	EEPCurrent->arg2 = 80;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-0A */
	strcpy(EEPCurrent->eep,"07020A");
	strcpy(EEPCurrent->name,"Capteur de température, entre 50°C et 90°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 50;
	EEPCurrent->arg2 = 90;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-0B */
	strcpy(EEPCurrent->eep,"07020B");
	strcpy(EEPCurrent->name,"Capteur de température, entre 60°C et 100°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 60;
	EEPCurrent->arg2 = 100;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-10 */
	strcpy(EEPCurrent->eep,"070210");
	strcpy(EEPCurrent->name,"Capteur de température, entre -60°C et 20°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -60;
	EEPCurrent->arg2 = 20;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-11 */
	strcpy(EEPCurrent->eep,"070211");
	strcpy(EEPCurrent->name,"Capteur de température, entre -50°C et 30°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -50;
	EEPCurrent->arg2 = 30;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-12 */
	strcpy(EEPCurrent->eep,"070212");
	strcpy(EEPCurrent->name,"Capteur de température, entre -40°C et 40°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -40;
	EEPCurrent->arg2 = 40;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-13 */
	strcpy(EEPCurrent->eep,"070213");
	strcpy(EEPCurrent->name,"Capteur de température, entre -30°C et 50°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -30;
	EEPCurrent->arg2 = 50;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-14 */
	strcpy(EEPCurrent->eep,"070214");
	strcpy(EEPCurrent->name,"Capteur de température, entre -20°C et 60°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -20;
	EEPCurrent->arg2 = 60;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-15 */
	strcpy(EEPCurrent->eep,"070215");
	strcpy(EEPCurrent->name,"Capteur de température, entre -10°C et 70°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = -10;
	EEPCurrent->arg2 = 70;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-16 */
	strcpy(EEPCurrent->eep,"070216");
	strcpy(EEPCurrent->name,"Capteur de température, entre 0°C et 80°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 0;
	EEPCurrent->arg2 = 80;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-17 */
	strcpy(EEPCurrent->eep,"070217");
	strcpy(EEPCurrent->name,"Capteur de température, entre 10°C et 90°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 10;
	EEPCurrent->arg2 = 90;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-18 */
	strcpy(EEPCurrent->eep,"070215");
	strcpy(EEPCurrent->name,"Capteur de température, entre 20°C et 100°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 20;
	EEPCurrent->arg2 = 100;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-19*/
	strcpy(EEPCurrent->eep,"070219");
	strcpy(EEPCurrent->name,"Capteur de température, entre 30°C et 110°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 30;
	EEPCurrent->arg2 = 110;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-1A*/
	strcpy(EEPCurrent->eep,"07021A");
	strcpy(EEPCurrent->name,"Capteur de température, entre 40°C et 120°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 40;
	EEPCurrent->arg2 = 120;		

	EEPCurrent = EEPCurrent->next;

	/* Insert EEP 07-02-1B*/
	strcpy(EEPCurrent->eep,"07021B");
	strcpy(EEPCurrent->name,"Capteur de température, entre 50°C et 130°C\0");
	EEPCurrent->AddSensors = AddSensorsTemp;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	EEPCurrent->arg1 = 50;
	EEPCurrent->arg2 = 130;		

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
	strcpy(EEPCurrent->name,"Capteur de lumière, température et présence [0-510lx][0-51°C]\0");
	EEPCurrent->AddSensors = NULL;	
	EEPCurrent->next = (EEP*)malloc(sizeof(EEP));
	
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

int AddSensorByEEP(char id[8], Sensor ** pp_sensorList, EEP* EEPList, char org[3], char funct[3], char type[3]){
	char eep[6];
	strcpy(eep,org);
	strcat(eep,funct);
	strcat(eep,type);
	while (EEPList != NULL){
		if (strcmp(EEPList->eep,eep) != 0)
		{
			EEPList = EEPList->next;
		}else{
			if (EEPList->AddSensors == NULL){
				return -1;	/* EEP not supported yet */
			}
			EEPList->AddSensors(id, pp_sensorList, EEPList->arg1, EEPList->arg2);
			return 0;
		}
	}
	return -2;	/* EEP not found */
}

int destroyEEPList(EEP* p_EEPList){
	return 0;
}

int AddSensorsContact(char id[8], Sensor ** pp_sensorList, float arg1, float arg2){
	Sensor* p_sensor;
	p_sensor = (Sensor*) malloc(sizeof(Sensor));
	p_sensor->next = *pp_sensorList;
	*pp_sensorList = p_sensor;
	strcpy((*pp_sensorList)->id,id);
	(*pp_sensorList)->id[9] = 'e';
	(*pp_sensorList)->id[10] = 'C';
	(*pp_sensorList)->value = 0;
	(*pp_sensorList)->decodeMessage = decodeMessageContact;
	return 0;
}

int AddSensorsSwitch(char id[8], Sensor ** pp_sensorList, float arg1, float arg2){
	Sensor* p_sensor;
	p_sensor = (Sensor*) malloc(sizeof(Sensor));
	p_sensor->next = *pp_sensorList;
	*pp_sensorList = p_sensor;
	strcpy((*pp_sensorList)->id,id);
	(*pp_sensorList)->id[9] = 'e';
	(*pp_sensorList)->id[10] = 'S';
	(*pp_sensorList)->value = 0;
	(*pp_sensorList)->decodeMessage = decodeMessageSwitch;
	return 0;
}

int AddSensorsTemp(char id[8], Sensor ** pp_sensorList, float arg1, float arg2){
	Sensor* p_sensor;
	p_sensor = (Sensor*) malloc(sizeof(Sensor));
	p_sensor->next = *pp_sensorList;
	*pp_sensorList = p_sensor;
	strcpy((*pp_sensorList)->id,id);
	(*pp_sensorList)->id[9] = 'e';
	(*pp_sensorList)->id[10] = 'T';
	(*pp_sensorList)->value = 0;
	(*pp_sensorList)->rangeData = (Range*)malloc(sizeof(Range));
	(*pp_sensorList)->rangeData->rangeMin = arg1;
	(*pp_sensorList)->rangeData->rangeMin = arg2;	
	(*pp_sensorList)->value = 0;
	(*pp_sensorList)->decodeMessage = decodeMessageTemp;
	return 0;
}