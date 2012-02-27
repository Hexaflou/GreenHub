/***********************************************************************
 * 				Primitives GreenHub d'allocation mémoire
 *
 * Fourni un remplacement à free et malloc via les fonctions gfree et
 * gmalloc.
 *
 * 														Hexanome H4212
 * ********************************************************************/


 //___________________________________________________________INCLUDES

#include <stddef.h>
#include <stdio.h>

//___________________________________________________________TYPEDEF

typedef long Align;

union en_tete {
	struct {
		union en_tete *ptr;
		unsigned taille;
	}s;
	Align x;
};

typedef union en_tete En_tete;

//___________________________________________________________DEFINE

#define NALLOUE 1024


//___________________________________________________________STATICS

static En_tete base;
static En_tete *plibre=NULL;

//___________________________________________________________FONCTIONS

//Procédure de libération de mémoire (remplace free)

void gfree (void *pa)
{

	En_tete *pb, *p;

	pb = (En_tete *)pa -1;
	for (p=plibre; !(pb > p && pb < p->s.ptr); p=p->s.ptr)
		if (p >= p->s.ptr && (pb > p || pb < p->s.ptr))
			break;

	if (pb + pb->s.taille == p->s.ptr) {
		pb->s.taille += p->s.ptr->s.taille;
		pb->s.ptr = p->s.ptr->s.ptr;

	} else
		pb->s.ptr = p->s.ptr;

	if (p + p->s.taille == pb ) {
		p->s.taille += pb->s.taille ;
		p->s.ptr = pb->s.ptr;
	} else
		p->s.ptr = pb;
	plibre =p;
}

// Procédure d'extension mémoire (opt)

En_tete *plusmem(unsigned nu) {

	char *pc, *sbrk(int);
	En_tete *pu;

	if (nu < NALLOUE)
		nu=NALLOUE;
	pc=sbrk(nu * sizeof(En_tete));
	if (pc == (char *) -1 )
		return NULL;
	pu = (En_tete *) pc;
	pu->s.taille = nu;
	gfree((void*)(pu+1));
	return plibre;
}

//Procédure d'allocation mémoire (remplace malloc)

void *gmalloc (unsigned noctets)
{

En_tete *p, *pprec;

unsigned nunites;

nunites = (noctets + sizeof(En_tete) -1)/ sizeof(En_tete)+1 ;

if ((pprec = plibre) == NULL) {
	base.s.ptr = plibre = pprec = &base;
	base.s.taille = 0;
}

for (p= pprec->s.ptr; ; pprec = p, p = p->s.ptr) {
	if(p->s.taille >= nunites) {
		if (p->s.taille == nunites)
			pprec->s.ptr = p->s.ptr;
		else {
			p->s.taille -= nunites;
			p += p->s.taille;
			p->s.taille = nunites;
		}
		plibre = pprec;
		return (void *) (p+1);
	}
	if(p == plibre)
			if( (p = plusmem(nunites) ) == NULL)
				return NULL;

		}
	}










