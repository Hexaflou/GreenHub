#ifndef SIMULATION_RECEPTOR_ENOCEAN_H_
#define SIMULATION_RECEPTOR_ENOCEAN_H_

/***********************************************************************
 * 	Tache GreenHub simulant le récepteur EnOcean
 * 
 * Permet la création d'une tâche et d'une boite aux lettres, tous les 
 * messages placés dans la boite au lettre sont envoyés par notre tâche
 * comme si ces messages provenaient du récepteur EnOcean
 * Utilisation réservé à sensors
 * 
 * 							Hexanome H4212
 * ********************************************************************/

#include <mqueue.h> 

/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSimulationReceptorTaskInit();

/* Destruction de la tache */
int comSimulationReceptorTaskClose();

#endif /* SIMULATION_RECEPTOR_ENOCEAN_H_ */
