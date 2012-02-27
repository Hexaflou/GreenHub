#ifndef COMRECEPTORTASK_H
#define COMRECEPTORTASK_H

/***********************************************************************
 * Tache GreenHub d�di� � l'�change des donn�es vers le r�cepteur EnOcean
 * 
 * Permet la cr�ation d'une tache et d'une boite au lettre, tous les 
 * messages plac�s dans la boite au lettre sont envoy� au r�cepteur 
 * EnOcean.
 * Utilisation r�serv� � sensors
 * 
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions externes */
#include <mqueue.h> 

/* Initialisation des deux t�ches, retourne un pointeur sur la boite au lettre */
mqd_t comReceptorTaskInit();

/* Destruction des deux t�ches */
int comReceptorTaskClose();

#endif /* COMRECEPTORTASK_H */
