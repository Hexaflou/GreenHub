#ifndef H_GLOGS
#define H_GLOGS
/***********************************************************************
 * 				Fonctions GreenHub de sauvegarde des données
 * 
 * Fourni via de simples fonctions de quoi sauvegarder de manière 
 * sécurisé les données récupérés des capteurs.
 * 
 * 														Hexanome H4212
 * ********************************************************************/

/* maybe will get a config filename in the future */
int gLogsInit();

/* don't forget need for exemple to kill the logs process */
int gLogsClose();

/* the unique identifier of the sensor and the getting value */
int gLogsLog(char mac[40], double value);


#endif
