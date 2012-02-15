#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gCommunication/gCommunication.h"
#include "gLogs.h"
#include "sensors/ComponentInterface.h"

int main ()
{
	gCommunicationInit(2);
	/* initialize random seed: */
	srand ( time(NULL) );
	gLogThreadInit();
	ComponentInterface((void*) NULL);
	getchar();
	gLogThreadClose();
	gCommunicationClose();
	return 0;
}
