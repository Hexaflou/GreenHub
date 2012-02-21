#ifndef H_GCONTEXT
#define H_GCONTEXT
/***********************************************************************
 *				Structure de sauvegarde de contexte (et thread)
 * 
 * Fourni pour l'ensemble du kernel Gthread la structure stockant les
 * donnee d'un thread
 * 
 *														Hexanome H4212
 *********************************************************************/

typedef enum{RUNNING, WAITING, SLEEPING, ACTIVABLE} gThread_state;

typedef struct
{
	int id;
	void * esp;	
	void * ebp;
	gThread_state state;
	void * stack;
	int wake_up_date;

	/* Pointer to the next task */
	ctx_s * next;
	
} ctx_s;

#endif
