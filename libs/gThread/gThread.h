#ifndef H_GTHREAD
#define H_GTHREAD
/***********************************************************************
 *						Primitives GThread
 * 
 * Ce fichier fournis l'ensemble des primitives publics du gestion 
 * des threads pour GreenHub. Ces threads sont indépendant du système
 * Les fonctionnalités blocantes verouillent donc complètement
 * l'application.
 * 
 *														Hexanome H4212
 *********************************************************************/
#include "context.h"

/* demarre le scheduler avec les taches cree. Rend la main que si toutes
 * les taches meurent. */
void gThread_start();

/*create a gThread with the specified stack size and parameters*/
GThread gThread_create(int stack_size, gfct* func, void * attr);

/* tue le thread donnee en paramètre (attention aux allocations)*/
int gThread_cancel(GThread);

/* rend la main à l'ordonanceur pour l'execution d'une autre tache*/
void yield();

/* Le gThread qui exécute cette procedure ne sera pas execute
 * avant le nombre de secondes passe en parametre */
void gSleep(int sec);

/* Desactive le thread */
void suspend(GThread thread);

/* Reactive le thread */
void active(GThread thread);

#endif
