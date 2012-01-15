
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
		  "=r"(pctx->SP) /* variables de sortie */
		: /* pas de variables d'entrée*/
		: );
	
	f(args);
	
	return 0;
}


void throw(struct ctx_s *pctx, int r)
{
	asm("movl %0, %%eax;" "\n\t"/* Second argument is return value.  */
		/* Save the return address now.  */
		"movl %5, %%ecx;" "\n\t"
		/* Restore registers.  */
		"movl %1, %%ebx;" "\n\t"
		"movl %2, %%esi;" "\n\t"
		"movl %3, %%edi;" "\n\t"
		"movl %4, %%ebp;" "\n\t"
		"movl %6, %%esp;" "\n\t"
		/* Jump to saved PC.  */
		"jmp *%%ecx;"
		:
		: "r"(r),"r"(pctx->ebx),"r"(pctx->esi),
		  "r"(pctx->edi),"r"(pctx->ebp),"r"(pctx->PC),
		  "r"(pctx->SP) /* variables d'entree */
		: );
}


