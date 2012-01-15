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
	long int * SP;
	long int * PC;/* next instruction to execute*/

	int retValue;
};


int try(struct ctx_s *pctx, func_t *f, void * args);


void throw(struct ctx_s *pctx, int r);


#endif
