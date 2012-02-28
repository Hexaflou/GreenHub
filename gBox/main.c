#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gCommunication/gCommunication.h"
#include "gLogs.h"
#include "sensors/ComponentInterface.h"

int run = 1;

void gBox()
{
	/* TODO : ce chiffre 2 doit venir d'un fichier de config */
	while(gCommunicationInit(2)!=0)
	{
		printf("[gBox] Nouvelle tentative de connexion dans 10 secondes...\n");
		sleep(10);
	}
    
    /* initialize random seed: */
    srand(time(NULL));
    gLogThreadInit();
    ComponentInterfaceInit();
    printf("[gBox] Application lancée, un appui sur ENTREE quittera l'application\n");
    getchar();
    run = 0;
    printf("[gBox] Fermeture normale de l'application\n");
    ComponentInterfaceClose();
    gLogThreadClose();
    gCommunicationClose();
}

int main() {
	
	gBox();
    return 0;
}
