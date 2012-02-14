#include "Component.h" 
#include "EEP.h"
#include "string.h"
#include <stdlib.h>
#include "cJSON.h"
#include <stdio.h>

#define TAILLE_EEP 76

char* createCSON(char* EEP, char* name){

	cJSON *root ; 
	root=cJSON_CreateObject();
	cJSON_AddStringToObject(root,"EEP",EEP);
	cJSON_AddStringToObject(root,"Name",name);
	return cJSON_Print(root);
}


/* EEPList has to be initialized before calling this function */
int initializeEEPList(EEP* EEPList)
{	
	char* eep;
	
	FILE *f = fopen("eep.txt", "w");

	eep = createCSON("050201","Interrupteur � 2 boutons, installe a l etat 0" );
	fprintf(f,eep);
	
	eep= createCSON("050202","Interrupteur � 2 boutons, installe a l etat 1" );
	fprintf(f,eep);
	
	eep= createCSON("050301","Interrupteur � 4 boutons, installe a l etat 0" );
	fprintf(f,eep);
	
	eep= createCSON("050401","Interrupteur a insertion de carte" );
	fprintf(f,eep);
	
	eep= createCSON("051000","Capteur de poignee de fenetre" );
	fprintf(f,eep);
	
  	eep= createCSON("060001","Capteur de poignee de fenetre" );
	fprintf(f,eep);
  
  	eep= createCSON("070201","Capteur de temp�rature, entre -40�C et 0�C\0" );
	fprintf(f,eep);
  
  	eep= createCSON("070202","Capteur de temp�rature, entre -30�C et 10�C" );
	fprintf(f,eep);
	
	eep= createCSON("070203","Capteur de temp�rature, entre -20�C et 20�C" );
	fprintf(f,eep);
	
	eep= createCSON("070204","Capteur de temp�rature, entre -10�C et 30�C" );
	fprintf(f,eep);
	
	eep= createCSON("070205","Capteur de temp�rature, entre 0�C et 40�C" );
	fprintf(f,eep);
	
	eep= createCSON("070206","Capteur de temp�rature, entre 10�C et 50�C\0" );
	fprintf(f,eep);
	
	eep= createCSON("070207","Capteur de temp�rature, entre 20�C et 60�C" );
	fprintf(f,eep);
	
	eep= createCSON("070208","Capteur de temp�rature, entre 30�C et 70�C" );
	fprintf(f,eep);
	
	eep= createCSON("070209","Capteur de temp�rature, entre 40�C et 80�C" );
	fprintf(f,eep);
	
	eep= createCSON("07020A","Capteur de temp�rature, entre 50�C et 90�C" );
	fprintf(f,eep);
	
	eep= createCSON("07020B","Capteur de temp�rature, entre 60�C et 100�C" );
	fprintf(f,eep);

	eep= createCSON("070210","Capteur de temp�rature, entre -60�C et 20�C" );
	fprintf(f,eep);
	
	eep= createCSON("070211","Capteur de temp�rature, entre -50�C et 30�C" );
	fprintf(f,eep);

	eep= createCSON("070212","Capteur de temp�rature, entre -40�C et 40�C" );
	fprintf(f,eep);

	eep= createCSON("070213","Capteur de temp�rature, entre -30�C et 50�C" );
	fprintf(f,eep);
	
	eep= createCSON("070214","Capteur de temp�rature, entre -20�C et 60�C" );
	fprintf(f,eep);
	
	eep= createCSON("070215","Capteur de temp�rature, entre -10�C et 70�C" );
	fprintf(f,eep);
	
	eep= createCSON("070216","Capteur de temp�rature, entre 0�C et 80�C" );
	fprintf(f,eep);
	
	eep= createCSON("070217","Capteur de temp�rature, entre 10�C et 90�C" );
	fprintf(f,eep);
	
	eep= createCSON("070215","Capteur de temp�rature, entre 20�C et 100�C" );
	fprintf(f,eep);
	
	eep= createCSON("070219","Capteur de temp�rature, entre 30�C et 110�C" );
	fprintf(f,eep);
	
	eep= createCSON("07021A","Capteur de temp�rature, entre 40�C et 120�C" );
	fprintf(f,eep);

	eep= createCSON("07021B","Capteur de temp�rature, entre 50�C et 130�C" );
	fprintf(f,eep);

	eep= createCSON("070401","Capteur de temp�rature et d'humidit� [0-40�C] [0-100%]" );
	fprintf(f,eep);
	
	eep= createCSON("070601","Capteur de luminosit�, entre 300 et 60000lx" );
	fprintf(f,eep);
	
	eep= createCSON("070602","Capteur de luminosit�, entre 0 et 1024lx" );
	fprintf(f,eep);
	
	eep= createCSON("070701","Capteur de pr�sence" );
	fprintf(f,eep);
	
	eep= createCSON("070801","Capteur de lumi�re, temp�rature et pr�sence [0-510lx][0-51�C]" );
	fprintf(f,eep);
	
	eep= createCSON("070802","Capteur de lumi�re, temp�rature et pr�sence [0-1020lx][0-51�C]" );
	fprintf(f,eep);

	eep= createCSON("070803","Capteur de lumi�re, temp�rature et pr�sence [0-1530lx][-30-50�C]" );
	fprintf(f,eep);
	
	eep= createCSON("070901","Capteur de gaz" );
	fprintf(f,eep);
	
	eep= createCSON("070904","Capteur de CO2" );
	fprintf(f,eep);
	
	eep= createCSON("070908","Capteur de O2" );
	fprintf(f,eep);
	
	eep= createCSON("07090C","Capteur de propane" );
	fprintf(f,eep);
	
	fclose(f);

	return 0;
}
