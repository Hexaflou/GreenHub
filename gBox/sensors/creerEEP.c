#include "Sensor.h"
#include "EEP.h"
#include "string.h"
#include <stdlib.h>
#include "../lib/cJSON.h"
#include <stdio.h>

#define TAILLE_EEP 76

char* createCJSONFirstEEP(char* EEP, char* name) {

	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "EEP", EEP);
	cJSON_AddStringToObject(root, "Name", name);
	return cJSON_Print(root);
}

/* EEPList has to be initialized before calling this function */
int initializeEEPListFirst() {
	char* eep;

	FILE *f = fopen("eep.txt", "w");

	eep = createCJSONFirstEEP("050201", "Interrupteur à 2 boutons, installe a l etat 0");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("050202", "Interrupteur à 2 boutons, installe a l etat 1");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("050301", "Interrupteur à 4 boutons, installe a l etat 0");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("050401", "Interrupteur a insertion de carte");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("051000", "Capteur de poignee de fenetre");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("060001", "Capteur de poignee de fenetre");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070201", "Capteur de temp�rature, entre -40�C et 0�C\0");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070202", "Capteur de temp�rature, entre -30�C et 10�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070203", "Capteur de temp�rature, entre -20�C et 20�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070204", "Capteur de temp�rature, entre -10�C et 30�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070205", "Capteur de temp�rature, entre 0�C et 40�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070206", "Capteur de temp�rature, entre 10�C et 50�C\0");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070207", "Capteur de temp�rature, entre 20�C et 60�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070208", "Capteur de temp�rature, entre 30�C et 70�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070209", "Capteur de temp�rature, entre 40�C et 80�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("07020A", "Capteur de temp�rature, entre 50�C et 90�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("07020B", "Capteur de temp�rature, entre 60�C et 100�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070210", "Capteur de temp�rature, entre -60�C et 20�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070211", "Capteur de temp�rature, entre -50�C et 30�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070212", "Capteur de temp�rature, entre -40�C et 40�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070213", "Capteur de temp�rature, entre -30�C et 50�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070214", "Capteur de temp�rature, entre -20�C et 60�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070215", "Capteur de temp�rature, entre -10�C et 70�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070216", "Capteur de temp�rature, entre 0�C et 80�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070217", "Capteur de temp�rature, entre 10�C et 90�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070215", "Capteur de temp�rature, entre 20�C et 100�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070219", "Capteur de temp�rature, entre 30�C et 110�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("07021A", "Capteur de temp�rature, entre 40�C et 120�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("07021B", "Capteur de temp�rature, entre 50�C et 130�C");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070401", "Capteur de temp�rature et d'humidit� [0-40�C] [0-100%]");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070601", "Capteur de luminosit�, entre 300 et 60000lx");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070602", "Capteur de luminosit�, entre 0 et 1024lx");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070701", "Capteur de pr�sence");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070801", "Capteur de lumi�re, temp�rature et pr�sence [0-510lx][0-51�C]");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070802", "Capteur de lumi�re, temp�rature et pr�sence [0-1020lx][0-51�C]");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070803", "Capteur de lumi�re, temp�rature et pr�sence [0-1530lx][-30-50�C]");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070901", "Capteur de gaz");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070904", "Capteur de CO2");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("070908", "Capteur de O2");
	fprintf(f, "%s", eep);

	eep = createCJSONFirstEEP("07090C", "Capteur de propane");
	fprintf(f, "%s", eep);

	fclose(f);

	return 0;
}
