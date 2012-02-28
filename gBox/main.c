#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "gCommunication/gCommunication.h"
#include "gLogs.h"
#include "sensors/ComponentInterface.h"

static int run = 1;
static int pid = 0;

void sighandler(int signum)
{
	run = 0;
	kill(pid,SIGTERM);
}

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
    kill(getppid(),SIGINT);
    exit(0);
}

int main() {
	
	int statut;
    int options = 0;
    
    signal(SIGINT,&sighandler);
    
	/* une sorte de watchdog relance l'appli si elle fini inopinement */
	while(run) {
		pid = 0;
		pid = fork();
		if(pid > 0){
			/*Code du pere*/
			waitpid(pid, &statut, options);
		}
		else if(pid==0) {
			gBox();
		}
	}

    return 0;
}
