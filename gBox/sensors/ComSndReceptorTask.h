#ifndef COMSNDRECEPTORTASK_H
#define COMSNDRECEPTORTASK_H
/***********************************************************************
 * 	Tache GreenHub d�di� � l'envoi des donn�es vers le r�cepteur EnOcean
 * 
 * Permet la cr�ation d'une tache et d'une boite au lettre, tous les 
 * messages plac�s dans la boite au lettre sont envoy� au r�cepteur 
 * EnOcean.
 * Utilisation r�serv� � sensors
 * 
 * 														Hexanome H4212
 * ********************************************************************/

#include <mqueue.h> 

 
/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSndReceptorTaskInit(int socket);

/* Destruction de la tache */
int comSndReceptorTaskClose();

#endif /* COMSNDRECEPTORTASK_H */
