#ifndef COMRECEPTORTASK_H
#define COMRECEPTORTASK_H
/***********************************************************************
 * Tache GreenHub dédié à l'échange des données vers le récepteur EnOcean
 * 
 * Permet la création d'une tache et d'une boite au lettre, tous les 
 * messages placés dans la boite au lettre sont envoyé au récepteur 
 * EnOcean.
 * Utilisation réservé à sensors
 * 
 * 							Hexanome H4212
 * ********************************************************************/

#include <mqueue.h> 

/* Initialisation des deux tâches, retourne un pointeur sur la boite au lettre */
mqd_t comReceptorTaskInit();

/* Destruction des deux tâches */
int comReceptorTaskClose();

#endif /* COMRECEPTORTASK_H */
