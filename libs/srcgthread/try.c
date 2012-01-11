
#include "try.h"
#include <stdio.h>


int try(struct ctx_s *pctx, func_t *f, int arg)
{
	asm ("movl %1, %%eax" "\n\t" "movl %%eax, %0"
: "=r"(y) /* y is output operand */
: "r"(x) /* x is input operand */
: "%eax"); /* %eax is a clobbered register */

  movl 4 (%esp), %eax
28	    /* Save registers. */
29	    movl %ebx, (0 *4)(%eax)
30	    movl %esi, (1 *4)(%eax)
31	    movl %edi, (2 *4)(%eax)
32	    /* Save SP as it will be after we return. */
33	    leal 4 (%esp), %ecx
34	    movl %ecx, (4 *4)(%eax)
35	    /* Save PC we are returning to now. */
36	    movl 0 (%esp), %ecx
37	    movl %ecx, (5 *4)(%eax)
38	    /* Save caller's frame pointer. */
39	    movl %ebp, (3 *4)(%eax) 
	
	/*
	int res = 0;
	pctx->errno = setjmp(pctx->buf);

	if(pctx->errno==0)
	{	
		printf("test\n");
		res = f(arg);
	}
	return res;*/
}


void throw(struct ctx_s *pctx, int r)
{
	/*
	longjmp(pctx->buf,r);
	*/
}


