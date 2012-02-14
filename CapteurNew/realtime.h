#ifndef H_GREALTIME
#define H_GREALTIME
/***********************************************************************
 * 						Tâche GreenHub RealTime
 * 
 * Cette tâche permet un log regulier des donnees récupérés
 * 
 * 														Hexanome H4212
 * ********************************************************************/
 
 /* initialise la tache de log periodique des donnees*/
 int gRT_init(int period);
 
 /* change la frequence de log du mode temps reel */
 int gRT_setPeriod(int period);
 
 /* Kill de la tâche de log periodique des donnees */
 int gRT_Close();
 
 #endif
