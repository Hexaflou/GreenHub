#ifndef H_COMRCVTASK
#define H_COMRCVTASK
/***********************************************************************
 * 				Tache GreenHub dédié à l'envoi des données
 * 
 * Permet la création d'une tache et d'une boite au lettre, tous les 
 * messages placés dans la boite au lettre sont envoyé au serveur.
 * Utilisation réservé à gCommunication
 * 
 * 														Hexanome H4212
 * ********************************************************************/

/* Initialisation de la tache */
int comRcvTaskInit(int socket);

/* Destruction de la tache */
int comRcvTaskClose();

#endif
