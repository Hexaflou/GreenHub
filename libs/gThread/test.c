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
		yield();
		puts("D");
		puts("E");
		puts("F");
		yield();
		puts("G");
		yield();
	}
}

void f2(void *args)
{
	while(go)
	{
		puts("1");
		puts("2");
		yield();
		puts("3");
		puts("4");
		puts("5");
		puts("6");
		yield();
		puts("7");
		puts("8");
		puts("9");
		yield();
	}
}


void f3(void *args)
{
	while(go)
	{
		puts("#");
		yield();
	}
}

int main()
{
	printf("Début de l'applicatiton de test\n");
	
	gThread_create(10000, f1, NULL);
	gThread_create(10000, f2, NULL);
	gThread_create(10000, f3, NULL);
	
	while(getchar()!='q')
	{
		sleep(1);
		yield();
	}
	go = 0;
	printf("Fin de l'application de test\n");
	return 0;
}
