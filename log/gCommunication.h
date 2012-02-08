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

#define SRV_IP "134.214.221.25"
#define SRV_PORT 9999

/* maybe will get a config filename in the future */
int gCommunicationInit(int userId);

/* don't forget need for exemple to kill the logs process */
int gCommunicationClose();

int gCommunicationSend(char * msg);

/* the unique identifier of the sensor and the getting value */
int gLogsLog(char mac[40], double value);


#endif

