#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "try.h"

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

static struct ctx_s * pctx;

int test(int i)
{
	if(i==1)
	{
		printf("Pas de throw\n");
	}
	else
	{
		printf("un trow\n");
		throw(pctx,i);
	}
	
	return i;
}

int main()
{
	int i = 0;
	
	pctx = malloc(sizeof(struct ctx_s));
	
	scanf("%d",&i);
	printf("Tu as rentre %d\n", try(pctx,test,i));
	if(pctx->errno)
		printf("Une erreur est survenue num : %d",pctx->errno);
	free(pctx);
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

