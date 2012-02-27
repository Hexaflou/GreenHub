#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gThread.h"

static int go = 1;

void f1(void *args)
{
	while(go)
	{
		puts("A");
		puts("B");
		puts("C");
		puts("D");
		puts("E");
		puts("F");
		puts("G");
	}
}

void f2(void *args)
{
	while(go)
	{
		puts("1");
		puts("2");
		puts("3");
		puts("4");
		puts("5");
		puts("6");
		puts("7");
		puts("8");
		puts("9");
	}
}


void f3(void *args)
{
	while(go)
	{
		puts("#");
	}
}

int main()
{
	printf("Début de l'applicatiton de test\n");
	
	gThread_create(10000, f1, NULL);
	gThread_create(10000, f2, NULL);
	gThread_create(10000, f3, NULL);
	
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
