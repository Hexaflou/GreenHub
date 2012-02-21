#include <stdio.h>
#include <stdlib.h>

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
	printf("Début de l'applicaiton de test\n");
	

	getchar();
	go = 0;
	printf("Fin de l'application de test\n");
	return 0;
}
