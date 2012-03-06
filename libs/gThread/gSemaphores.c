#include "gSemaphores.h"
#include "gThread.h"
#include <hw.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int value;
} GSem;


 int gsem_create(int value)
 {
	 return (int) malloc(sizeof(GSem));
 }
 
 void gsem_give(int sem)
 {
	 irq_disable();
	 ((GSem *) sem )->value++;
	 irq_enable();
 }
 
 void gsem_take(int sem)
 {
	 irq_disable();
	 while(((GSem *) sem )->value==0)
	 {
		yield();
		irq_disable();
	 }
		
	 ((GSem *) sem )->value--;
	 irq_enable();
 }
 
 void gsem_delete(int sem)
 {
	 irq_disable();
	 while(((GSem *) sem )->value<10)
	 {
		 gsem_give(sem);
		 yield();
		 irq_disable();
	 }
		
	 free((GSem*)sem);
	 irq_enable();
 }
