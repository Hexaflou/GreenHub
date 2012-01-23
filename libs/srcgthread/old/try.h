#ifndef H_GTRY
#define H_GTRY
/***********************************************************************
 * 				Primitives GreenHub de d'exécution protégé (try, throw)
 * 
 * Fourni des fonctions try et throw, le contexte sauvegardé est local.
 * 
 * 														Hexanome H4212
 * ********************************************************************/

#include <setjmp.h>
typedef void (func_t)(void *);/* prototype des taches */

struct ctx_s
{
	/* sauvegarde de la plupart des registres necessaires */
	long int   ebx;
	long int   esi;
	long int   edi;
	long int * esp;/* esp */	
	long int * ebp;/*for setjump, caller frame pointer*/
	long int * PC;/* next instruction to execute*/

	int retValue;
};

/* for correct use in asm code*/
# define CTX_BX    "0"
# define CTX_SI    "1"
# define CTX_DI    "2"
# define CTX_SP    "3"
# define CTX_BP    "4"
# define CTX_PC    "5"
# define CTX_SIZE 26


int try(struct ctx_s *pctx, func_t *f, void * args);


void throw(struct ctx_s *pctx, int r);


#endif
