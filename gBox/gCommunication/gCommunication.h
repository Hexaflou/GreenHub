#ifndef H_GCOMMUNICATION
#define H_GCOMMUNICATION
/***********************************************************************
 *            Fonctions GreenHub de sauvegarde des données
 * 
 * Fourni via de simples fonctions de quoi sauvegarder de manière 
 * sécurisé les données récupérés des capteurs.
 * 
 *                                                      Hexanome H4212
 * ********************************************************************/

#define SRV_IP "greenhub.signez.fr"
#define SRV_PORT 1863

/* maybe will get a config filename in the future */
int gCommunicationInit(int userId);

/* don't forget need for exemple to kill the logs process */
int gCommunicationClose();

/* send data to the server (thread-safe) */
int gCommunicationSend(char * msg);

/* send value to the server (thread-safe) */
int gCommunicationSendValue(char hwid[12], double value);

/* return 1 if yes and 0 if no */
int gCommunicationIsAlive();



#endif

