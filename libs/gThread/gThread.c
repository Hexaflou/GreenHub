#include "context.h"
#include "gThread.h"
#include <hw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*****************************PRIVATE DECLARATION*****************************/
/* id of the current thread */
static GThread current_thread = 0;
static ctx_s * first_thread = 0;
static int next_id = 0;
void delete_current_thread();
int delete_thread(GThread todelete);
void switch_ctx(ctx_s* to_save);
void first_launch(ctx_s* to_save);

/*****************************PUBLIC FUNCTIONS********************************/
void gThread_start()
{
	start_hw();
	setup_irq(TIMER_IRQ, yield);
}

GThread gThread_create(int stack_size, gfct* func, void * attr)
{

	ctx_s * athread = (ctx_s *)malloc(sizeof(ctx_s));
	athread->id = next_id++;
	athread->stack = (void *) malloc(stack_size + 32);/* Il y a un peu de marge TODO:Ajuster*/
	athread->wake_up_date = 0;
	athread->state = TOLAUNCH;

	/* ESP et EBP pointent sur la sauvegarde de EBP */
	athread->esp = athread->ebp = athread->stack + stack_size;
	
	athread->func=func;
	athread->attr=attr;
	
	/* insertion en tête */
	athread->last = NULL;
	if (first_thread == NULL)
	{
		first_thread = athread;
		athread->next = NULL;
		
	}
	else
	{
		athread->next = first_thread;
		first_thread->last = athread;
		first_thread = athread;
	}
	return (GThread) athread;
}

int gThread_cancel(GThread athread)
{
	ctx_s * th = first_thread;
	while(th->next != athread)
	{
		th = th->next;
	}
	/* on vire le thread de la liste des threads */
	th->next = athread->next;
	free(((ctx_s *) athread)->stack);
	free((ctx_s *) athread);
	return 0;
}

void yield()
{
	ctx_s * old=NULL;
	ctx_s * athread=NULL;
	irq_disable();
	/* Si le thread courant est null -> c'est le principal,
	 *  il faut le creer */
	if(current_thread==NULL)
	{
		current_thread = malloc(sizeof(ctx_s));
		current_thread->id = next_id++;
		current_thread->stack = NULL;
		current_thread->wake_up_date = 0;
		current_thread->next=first_thread;
		/* insertion en tete */
		current_thread->last=NULL;
		first_thread->last = current_thread;
		first_thread=current_thread;
		current_thread->state = ACTIVABLE;
	}
	old = current_thread;
	/* si il n'y a qu'un thread c'est que c'est le courant...il n'y a 
	 * rien a faire, on y est */
	if(first_thread->next == NULL)
	{
		irq_enable();
		return;
	}
	
	/* si le thread courant n'a pas été mis en pause, il est activable */
	if(current_thread->state == RUNNING)
		current_thread->state = ACTIVABLE;
	
	
	/* On cherche le thread activable suivant */
	do {
		/* Si on est au dernier, on recommence au debut*/
		if(current_thread->next == NULL)
		{
			current_thread = first_thread;
		}
		else
		{
			current_thread = current_thread->next;
		}
		/* Verification pour les sleep */
		if(current_thread->state == SLEEPING && 
								time(NULL) > current_thread->wake_up_date)
		{
			current_thread->wake_up_date=0;
			current_thread->state=ACTIVABLE;
		}
		
		/* Verification pour les suppressions */
		if(current_thread->state == TODELETE)
		{
			athread = current_thread;
			if(current_thread->next == NULL)
				current_thread = first_thread;
			else
				current_thread = current_thread->next;
			delete_thread(athread);
		}
		
	} while(   current_thread->state != ACTIVABLE 
			&& current_thread->state != TOLAUNCH  );
	
	
	/* Premier ou deuxieme lancement ?*/
	if(current_thread->state==TOLAUNCH)
	{
		current_thread->state=RUNNING;
		first_launch(old);
	}
	else
	{
		current_thread->state=RUNNING;
		switch_ctx(old);
	}
	
	irq_enable();
}

void gSleep(int sec)
{
	irq_disable();
	current_thread->wake_up_date=time(NULL)+sec;
	current_thread->state=SLEEPING;
	yield();
}

void suspend(GThread thread)
{
	irq_disable();
	if(thread == first_thread)
	{
		first_thread = first_thread->next;
	}
	else if(thread->last != NULL)
	{
		thread->last->next = thread->next;
	}
	thread->last = NULL;
	thread->next = NULL;
	
	if(thread->state == TOLAUNCH)
	{
		thread->state = WAITINGT;
	}
	else if (thread->state == ACTIVABLE)
	{
		thread->state = WAITING;
	}
	irq_enable();
}

void active(GThread thread)
{
	irq_disable();
	thread->next = (ctx_s *) first_thread;
	thread->last = NULL;
	first_thread->next = (ctx_s *) thread;
	if(thread->state == WAITINGT)
	{
		thread->state = TOLAUNCH;
	}
	else if (thread->state == WAITING)
	{
		thread->state = ACTIVABLE;
	}
	irq_enable();
}

GThread gGetpid()
{
	return current_thread;
}



/*****************************PRIVATE FUNCTIONS******************************/

int delete_thread(GThread todelete)
{
	if(todelete == NULL)
		return -1;
	
	if(current_thread == todelete)
	{
		current_thread->state = TODELETE;
		return 0;
	}
	
	/* On l'enleve de la liste */
	if(todelete == first_thread)
	{
		first_thread = first_thread->next;
		first_thread->last = NULL;
	}
	else if(todelete->last != NULL)
	{
		todelete->last->next = todelete->next;
	}
	free(todelete->stack);
	free(todelete);
	
	return 0;
}

void delete_current_thread()
{
	ctx_s * todelete;
	irq_disable();
	todelete = current_thread;
	if(current_thread == NULL)
		return;

	current_thread->state = TODELETE;
	yield();
}

void first_launch(ctx_s* to_save)
{
	asm("movl %%esp, %0" "\n" 
	    "movl %%ebp, %1"
       :"=r"(to_save->esp),
        "=r"(to_save->ebp) );
        
	asm("movl %0, %%esp" "\n"
	    "movl %1, %%ebp"
		:
		:"r"(current_thread->esp),
		 "r"(current_thread->ebp ));
	
	/* On reactive le yield periodique avant de lancer la tache */	 
	irq_enable();
	current_thread->func(current_thread->attr);
	
	/* au retour de la fonction on se retrouve ici on detruit
	 *  donc la tache */
	delete_current_thread();
}

void switch_ctx(ctx_s* to_save)
{
	asm("movl %%esp, %0" "\n" 
	    "movl %%ebp, %1"
       :"=r"(to_save->esp),
        "=r"(to_save->ebp) );
	asm("movl %0, %%esp" "\n"
	    "movl %1, %%ebp"
		:
		:"r"(current_thread->esp),
		 "r"(current_thread->ebp ));
		 
	/* absolument inutile si ce n'est que sans une operation autre 
	 * que l'assembleur le compilateur va nous ch*** la fonction */	 
	irq_enable();
	return;
}
