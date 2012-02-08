#ifndef H_GLOGTHREAD
#define H_GLOGTHREAD
/***********************************************************************
 * 				Fonctions GreenHub de sauvegarde des données
 * 
 * Fourni via de simples fonctions de quoi sauvegarder de manière 
 * sécurisé les données récupérés des capteurs.
 * 
 * 														Hexanome H4212
 * ********************************************************************/

#define LOG_SEND_PERIOD 	60
#define LOG_FILENAME 		"sensors.log"
#define LOG_STATE_FILENAME 	"sendstate.data"

/* Log the given datas */
int gLogsLog (char mac[40], double value);

int gLogThreadInit();

int gLogThreadClose();

#endif
