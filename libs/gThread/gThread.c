#include "context.h"
#include "gThread.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************PRIVATE DECLARATION*****************************/
/* id of the current thread */
static GThread current_thread = 0;
static ctx_s * first_thread = 0;
void delete_current_thread();
void switch_ctx(ctx_s* to_save, ctx_s* to_load);
void first_launch(ctx_s* to_save, ctx_s* to_load);

/*****************************PUBLIC FUNCTIONS********************************/
GThread gThread_create(int stack_size, gfct* func, void * attr)
{
	static int id = 0;
	ctx_s * athread = (ctx_s *)malloc(sizeof(ctx_s));
	athread->id = id++;
	athread->stack = (void *) malloc(stack_size + 32);/* Il y a un peu de marge TODO:Ajuster*/
	athread->wake_up_date = 0;
	athread->state = TOLAUNCH;
	
#ifdef UNACTIVE
	/* sauvegarde de l'attribut de*/
	*(long*)(athread->stack + stack_size + 16) = (long) attr;
	/* Adresse de esp restaure apres return du switch */
	*(long*)(athread->stack + stack_size) = (long)(athread->stack + stack_size + 8);
	/* Adresse de la fonction a exectuter (endroit du saut apres return du switch) */
	*(long*)(athread->stack + stack_size + 4) = (long)func;
	/* Adresse de la fonction sur retour depuis la tache */
	*(long*)(athread->stack + stack_size + 8) = (long)&delete_current_thread;
	/* Adresse de ebp */
	*(long*)(athread->stack + stack_size + 12) = (long)(athread->stack + stack_size + 12);
#endif

	/* ESP et EBP pointent sur la sauvegarde de EBP */
	athread->esp = athread->ebp = athread->stack + stack_size;
	
	athread->func=func;
	athread->attr=attr;
	
	/* insertion en tête */
	if (first_thread == NULL)
	{
		first_thread = athread;
		athread->next = NULL;
	}
	else
	{
		athread->next = first_thread;
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
	th->next = athread->next;/* on vire le thread de la liste des threads */
	free(((ctx_s *) athread)->stack);
	free((ctx_s *) athread);
	return 0;
}

void yield()
{
	ctx_s * old = current_thread;
	if(current_thread==NULL)
	{
		current_thread = malloc(sizeof(ctx_s));
		current_thread->id = 255;
		current_thread->stack = NULL;
		current_thread->wake_up_date = 0;
		current_thread->state = ACTIVABLE;
		current_thread->next=first_thread;
		first_thread=current_thread;
		old = current_thread;
	}
	if(current_thread->next == NULL)
	{
		current_thread=first_thread;
	}
	else
	{
		current_thread = current_thread->next;
	}

	if(current_thread->state==TOLAUNCH)
	{
		current_thread->state=ACTIVABLE;
		first_launch(old,current_thread);
	}
	else
	{
		switch_ctx(old,current_thread);
	}
}

void gSleep(int sec)
{

}

void suspend(GThread thread)
{

}

void active(GThread thread)
{

}

/*****************************PRIVATE FUNCTIONS******************************/

void delete_current_thread()
{
	ctx_s * todelete = current_thread;
	
	if(current_thread == NULL)
		return;
	
	if(current_thread == first_thread)
	{
		first_thread = first_thread->next;
	}
	free(todelete->stack);
	free(todelete);
	yield();
}

void first_launch(ctx_s* to_save, ctx_s* to_load)
{
	printf("first launch of thread number %d\n",to_load->id);
	asm("movl %%esp, %0" "\n" 
	    "movl %%ebp, %1"
       :"=r"(to_save->esp),
        "=r"(to_save->ebp) );
	asm("movl %0, %%esp" "\n"
	    "movl %1, %%ebp"
		:
		:"r"(to_load->esp),
		 "r"(to_load->ebp ));
	
	current_thread->func(current_thread->attr);
	
	/* if we are here...*/
	delete_current_thread();
}

void switch_ctx(ctx_s* to_save, ctx_s* to_load)
{
	asm("mov %%ebp, %0" :"=r"(to_save->ebp));
	asm("mov %%esp, %0" :"=r"(to_save->esp));
	asm("mov %0, %%esp" ::"r"(to_load->esp));
	asm("mov %0, %%ebp" ::"r"(to_load->ebp));
	return;
}
