#ifndef H_GTHREAD
#define H_GTHREAD
/***********************************************************************
 * 				Primitives GreenHub de gestion de threads
 * 
 * Fourni un ensemble de primitive de gestion et création de threads
 * 
 * 														Hexanome H4212
 * ********************************************************************/
 
 #define NAME_SIZE 20
 #define MIN_STACK_SIZE 32768
typedef void (gFunc_t)(void *);/* prototype des taches */

struct ctx_s
{
	long int * esp;	
	long int * ebp;
	void * args;
	gFunc_t * func;
	int launched;
};


int init_ctx(struct ctx_s *pctx, int stack_size,
								gFunc_t *f, void * args);


void switch_to_ctx(struct ctx_s *pctx);


#endif
