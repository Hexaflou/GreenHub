#ifndef H_COMSENDTASK
#define H_COMSENDTASK
/***********************************************************************
 * 				Tache GreenHub dédié à l'envoi des données
 * 
 * Permet la création d'une tache et d'une boite au lettre, tous les 
 * messages placés dans la boite au lettre sont envoyé au serveur.
 * Utilisation réservé à gCommunication
 * 
 * 														Hexanome H4212
 * ********************************************************************/
#include <mqueue.h>

/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSendTaskInit(int socket);

/* Destruction de la tache */
int comSendTaskClose();

void comSendUpdateSocket(int socket);

#endif
