
#include "try.h"
#include <stdio.h>


int try(struct ctx_s *pctx, func_t *f, void * args)
{
	asm ("movl %%esp, %0;" "\n\t"
		 "movl %%ebp, %1;" "\n\t"
		: "=r"(pctx->esp),"=r"(pctx->ebp) /* variables de sortie */
		: /* pas de variables d'entrée*/
		: );
	
	/*printf("esp : %ld ebp : %ld\n",(long int)pctx->esp,(long int)pctx->ebp);*/
	f(args);
	
	return 0;
}


void throw(struct ctx_s *pctx, int r)
{
	asm ("movl %0,%%esp ;" "\n\t"
		 "movl %1,%%ebp ;" "\n\t"
		 "movl %2,%%eax ;" "\n\t"
		 "leave ;" "\n\t"
		 "ret ;""\n\t"
		:
		: "r"(pctx->esp),"r"(pctx->ebp),"r"(r)/* variables d'entree */
		: "%eax");
		
}











#ifdef sav

int try(struct ctx_s *pctx, func_t *f, void * args)
{
	asm ("movl %%esp, %0;" "\n\t"
		 "movl %%ebp, %1;" "\n\t"
		 "movl %%ebx, %2;" "\n\t"
		 "movl %%esi, %3;" "\n\t"
		 "movl %%edi, %4;" "\n\t"
		: "=r"(pctx->esp),"=r"(pctx->ebp),"=r"(pctx->ebx),
			"=r"(pctx->esi),"=r"(pctx->edi) /* variables de sortie */
		: /* pas de variables d'entrée*/
		: );
	
	printf("esp : %ld ebp : %ld\n",(long int)pctx->esp,(long int)pctx->ebp);
	f(args);
	
	return 0;
}


int throw(struct ctx_s *pctx, int r)
{
	ret = r;
	asm ("movl %0,%%esp ;" "\n\t"
		 "movl %1,%%ebp ;" "\n\t"
		 "movl %2,%%ebx ;" "\n\t"
		 "movl %3,%%esi ;" "\n\t"
		 "movl %4,%%edi ;" "\n\t"
		:
		: "r"(pctx->esp),"r"(pctx->ebp),"r"(pctx->ebx),
			"r"(pctx->esi),"r"(pctx->edi)/* variables d'entree */
		: );
	
		printf("2 esp : %ld ebp : %ld\n",(long int)pctx->esp,(long int)pctx->ebp);
		return ret;
}

#endif
