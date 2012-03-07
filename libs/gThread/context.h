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

typedef enum{RUNNING, WAITING, WAITINGT, SLEEPING, ACTIVABLE, TOLAUNCH,TODELETE} gThread_state;
typedef void(gfct)(void *);

struct ctx_ss
{
	int id;
	long * esp;
	long * ebp;
	gThread_state state;
	long * stack;
	int wake_up_date;
	gfct * func;
	void * attr;

	/* Pointer to the next task */
	struct ctx_ss * next;
	/* Pointer to the last task */
	struct ctx_ss * last;
	
};

typedef struct ctx_ss ctx_s;

typedef ctx_s * GThread;

#endif
