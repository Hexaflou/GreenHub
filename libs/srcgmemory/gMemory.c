/***********************************************************************
 * 				Primitives GreenHub d'allocation mémoire
 * 
 * Fourni un remplacement à free et malloc via les fonctions gfree et
 * gmalloc. 
 * 
 * 														Hexanome H4212
 * ********************************************************************/
 //TODO : cette version utilise les fonctions standard !! temporaire.
 
 //___________________________________________________________INCLUDES
 
 
 //___________________________________________________________FONCTIONS
 
 //Procédure d'allocation mémoire (remplace malloc)
void *gmalloc (unsigned size) 
{
	return malloc(size);
}

//Procédure de libération de mémoire (remplace free)
void gfree (void *ptr) 
{
	free(ptr);
}
