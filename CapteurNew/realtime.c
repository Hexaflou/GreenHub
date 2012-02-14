#include "realtime.h"


#include <pthread.h>

/********************* LOCAL functions and variables *****************/

static int lPeriod;

void task();


/******************** PUBLIC functions *******************************/
 /* initialise la tache de log periodique des donnees*/
 int gRT_init(int period)
 {
	 lPeriod = period;
	 
	 return 0;
 }
 
 /* change la frequence de log du mode temps reel */
 int gRT_setPeriod(int period)
 {
	 return 0;
 }
 
 /* Kill de la tâche de log periodique des donnees */
 int gRT_Close()
 {
	 return 0;
 }


/**********************************************************************/

void task()
{

}



