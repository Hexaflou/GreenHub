#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gThread.h"
#include "gSemaphores.h"

static int go = 1;
static int sem;

void ping(void *args)
{
	while(go)
	{
		gsem_give(sem);
		puts("ping");
		gSleep(5);
	}
}

void pong(void *args)
{
	while(go)
	{
		gsem_take(sem);
		puts("pong");
	}
}


void f3(void *args)
{
	while(go)
	{
		puts("#");
		gSleep(2);
	}
}

int main()
{
	printf("Début de l'applicatiton de test\n");
	
	gThread_create(10000, ping, NULL);
	gThread_create(10000, pong, NULL);
	gThread_create(10000, f3, NULL);
	
	sem = gsem_create(0);
	
	gThread_start();
	
	while(getchar_unlocked()!='q')
	{
		/* Rendre la main tout de suite */
		yield();
	}
	go = 0;
	printf("attente de la mort des taches\n");
	
	printf("Fin de l'application de test\n");
	return 0;
}
