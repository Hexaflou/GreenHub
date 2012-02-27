#ifndef H_GSEMAPHORES
#define H_GSEMAPHORES
/***********************************************************************
 *				Fournis des semaphores
 * 
 * Fourni des semaphores fonctionnels pour gThread.
 * 
 *														Hexanome H4212
 *********************************************************************/
 
 int gsem_create(int value);
 
 void gsem_give(int sem);
 
 void gsem_take(int sem);
 
 /* becarefull of eventual crashes in case of future use */
 void gsem_delete(int sem);
 
 #endif
