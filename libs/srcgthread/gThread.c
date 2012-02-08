#include <stdlib.h>
#include <stdio.h>

#include "gThread.h"


/*static int lastId = 0;

static gThread *firstThread = NULL;*/
static struct ctx_s *currThread = NULL;

int init_ctx(struct ctx_s *pctx, int stack_size,
								gFunc_t *f, void * args)
{
	pctx->func = f;
	pctx->args = args;
	pctx->launched = 0;

	/*pctx->ebp = pctx->esp;
	pctx->esp = pctx->ebp-stack_size;*/
	
	return 0;
}


void switch_to_ctx(struct ctx_s *pctx)
{
	printf("switch\n");
	if(currThread!=NULL)
	{
		
		asm ("movl %%esp, %0" "\n\t"
		 "movl %%ebp, %1" "\n\t"
		: "=r"(currThread->esp),"=r"(currThread->ebp) 
		: 
		: );
		printf("save thread esp : %lu , ebp : %lu\n",
		(unsigned long int) currThread->esp,
		(unsigned long int) currThread->ebp);
	}
	else
	{
		printf("first thread\n");
	}
	currThread = pctx;
	if(pctx->launched == 0)
	{
		printf("first launch\n");
		pctx->launched = 1;
		pctx->func(pctx->args);
	}
	else
	{
		printf("restore ctx esp : %lu , ebp : %lu\n",
		(unsigned long int) pctx->esp,
		(unsigned long int) pctx->ebp);
		asm ("movl %1,%%ebp" "\n\t"
			"movl %0,%%esp " "\n\t"
		:
		: "r"(pctx->esp),"r"(pctx->ebp)
		: "%esp");
		return;
	}
}
