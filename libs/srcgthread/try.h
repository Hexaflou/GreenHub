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
typedef int (func_t)(int); /* a function that returns an int from an int */

struct ctx_s
{
	jmp_buf buf;
	int errno;
};


int try(struct ctx_s *pctx, func_t *f, int arg);


void throw(struct ctx_s *pctx, int r);


#endif
