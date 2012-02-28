#ifndef H_GMEMORY
#define H_GMEMORY
/***********************************************************************
 * 				Primitives GreenHub d'allocation mémoire
 *
 * Fourni un remplacement à free et malloc via les fonctions gfree et
 * gmalloc.
 *
 * 														Hexanome H4212
 * ********************************************************************/




/* Procédure d'allocation mémoire (remplace malloc) */
void *gmalloc (unsigned size) ;

/* Procédure de libération de mémoire (remplace free) */
void gfree (void *ptr) ;

#endif
