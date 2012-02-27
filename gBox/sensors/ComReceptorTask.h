#ifndef COMRECEPTORTASK_H
#define COMRECEPTORTASK_H

/***********************************************************************
 * Tache GreenHub dédié à l'échange des données vers le récepteur EnOcean
 * 
 * Permet la création d'une tache et d'une boite au lettre, tous les
 * messages placés dans la boite au lettre sont envoyée au récepteur
 * EnOcean.
 * Utilisation réservée à sensors
 * 
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions externes */
#include <mqueue.h> 

/* Initialisation des deux t�ches, retourne un pointeur sur la boite au lettre */
mqd_t comReceptorTaskInit(char *,int);

/* Destruction des deux t�ches */
int comReceptorTaskClose();

#endif /* COMRECEPTORTASK_H */
