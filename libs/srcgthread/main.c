#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "try.h"

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

static struct ctx_s pctx;

void test(void * i)
{
	if(* (int *)i==0)
	{
		printf("Pas de throw\n");
	}
	else
	{
		printf("un trow\n");
		throw(&pctx,* (int *)i);
	}
}

int main()
{
	int i = 0;
	
	
	scanf("%d",&i);
	printf("Tu as rentre %d\n", try(&pctx,test,(void*)&i));
	
	return 0;
}


/*int
main(int argc, char *argv[])
{
    create_ctx(16384, f_pong, NULL);
    create_ctx(16384, f_ping, NULL);
    start_sched();
    exit(EXIT_SUCCESS);
}*/

