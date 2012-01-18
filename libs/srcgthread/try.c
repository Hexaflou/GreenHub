
#include "try.h"
#include <stdio.h>


int try(struct ctx_s *pctx, func_t *f, void * args)
{
	asm (/* Save registers. */
		 "movl %%ebx, %0;" "\n\t"
		 "movl %%esi, %1;" "\n\t"
		 "movl %%edi, %2;" "\n\t"
		 /* Save SP as it will be after we return. */
		 "leal 4 (%%esp), %%ecx;" "\n\t"
		 "movl %%ecx, %5;" "\n\t"
		 /* Save PC we are returning to now. */
		 "movl 0 (%%esp), %%ecx;" "\n\t"
		 "movl %%ecx, %4;" "\n\t"
		 /* Save caller's frame pointer. */
		 "movl %%ebp, %3;" "\n\t"
		: "=r"(pctx->ebx),"=r"(pctx->esi),"=r"(pctx->edi),
		  "=r"(pctx->ebp),"=r"(pctx->PC),
		  "=r"(pctx->esp) /* variables de sortie */
		: /* pas de variables d'entrée*/
		: );
	
	f(args);
	
	return 0;
}


void throw(struct ctx_s *pctx, int r)
{
	asm ("movl %0, %%eax;" "\n\t"/* sauvegarde de la variable de retour*/
		"movl %1, %%ecx;" "\n\t"/* pointer vers le struct de contexte*/
		/* sauvegarde de l'adresse de retour */
		"movl ("CTX_PC"*4)(%%ecx), %%edx;" "\n\t"
		/* Restore registers. */
		"movl ("CTX_BX"*4)(%%ecx), %%ebx;" "\n\t"
		"movl ("CTX_SI"*4)(%%ecx), %%esi;" "\n\t"
		"movl ("CTX_DI"*4)(%%ecx), %%edi;" "\n\t"
		"movl ("CTX_BP"*4)(%%ecx), %%ebp;" "\n\t"
		"movl ("CTX_SP"*4)(%%ecx), %%esp;" "\n\t"
		"jmpl *%%edx;"
		:
		: "r"(r),"r"(pctx)/* variables d'entree */
		: "%eax","%ecx","%edx");
}


