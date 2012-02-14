#ifndef H_GCOMMUNICATION
#define H_GCOMMUNICATION
/***********************************************************************
 * 				Fonctions GreenHub de sauvegarde des données
 * 
 * Fourni via de simples fonctions de quoi sauvegarder de manière 
 * sécurisé les données récupérés des capteurs.
 * 
 * 														Hexanome H4212
 * ********************************************************************/


int gCommunicationParse(char * msg);
void sensorAction(char* mac_address,char * action);
void getValue(char * mac_address);
void activateRT();


#endif

