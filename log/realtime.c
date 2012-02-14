#include "realtime.h"
#include "semaphore.h"
#include "gCommunication.h"
#include "../CapteurNew/ComponentInterface.h"
#include "../CapteurNew/Component.h"
#include <pthread.h>

/********************* LOCAL functions and variables *****************/

static int lPeriod;
static pthread_t periodLog;
static sem_t semSensorList;
void gRealTimeLog();


/******************** PUBLIC functions *******************************/
 /* initialise la tache de log periodique des donnees*/
 int gRT_init(int period)
 {
	 lperiod = period;
	 periodLog = pthread_create( &periodLog, NULL, gRealTimeLog, (void *) NULL); 
	 return 0;
 }
 
 /* change la frequence de log du mode temps reel */
 int gRT_setPeriod(int period)
 {
	 
	 if( period == 0 && lperiod != 0 )
	 {
		 gRT_Close();
	 }
	 else if( lperiod == 0 && period != 0 )
	 {
		 gRT_init(period);
     }
	 else
     {
		 lperiod=period; 
     }
	 
	 return 0;
 }
 
 /* Kill de la tâche de log periodique des donnees */
 int gRT_Close()
 {
	 pthread_cancel(periodLog);
	 return 0;
 }


/**********************************************************************/

void gRealTimeLog()
{
	Sensor* tempSensor = NULL;
	
	tempSens=getSensorList();
	semSensorList = getSemaphore() ;
	
	sem_wait(&semSensorList);
	
	while ( lperiod != 0 )
	
	{
		
		while(tempSens != NULL)
		{
			gCommunicationSendValue(tempSensor->id,tempSens->value);
			tempSensor = tempSensor->next;
		}
		
		sem_post(&semSensorList);
		
		sleep(lperiod);
		
		tempSens=getSensorList();
		
	}
		

}



