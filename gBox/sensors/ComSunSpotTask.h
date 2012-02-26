#ifndef COMSUNSPOTTASK_H
#define COMSUNSPOTTASK_H

/***********************************************************************
 * Tache GreenHub dédié à l'échange des données avec les composants SunSpot
 *
 * Permet l'écoute de composants SunSpot pour un traitement futur
 * Utilisation réservé à sensors
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Initialisation de la tâche */
int comSunSpotTaskInit();

/* Destruction de la tâche */
int comSunSpotTaskClose();

#endif /* COMSUNSPOTTASK_H */
