#ifndef COMSNDRECEPTORTASK_H
#define COMSNDRECEPTORTASK_H
/***********************************************************************
 * 	Tache GreenHub dédié à l'envoi des données vers le récepteur EnOcean
 * 
 * Permet la création d'une tache et d'une boite au lettre, tous les 
 * messages placés dans la boite au lettre sont envoyé au récepteur 
 * EnOcean.
 * Utilisation réservé à sensors
 * 
 * 														Hexanome H4212
 * ********************************************************************/

#include <mqueue.h> 

 
/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSndReceptorTaskInit(int socket);

/* Destruction de la tache */
int comSndReceptorTaskClose();

#endif /* COMSNDRECEPTORTASK_H */
