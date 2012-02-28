/***********************************************************************
 * 				Primitives GreenHub d'allocation mémoire
 *
 * Fourni un remplacement à free et malloc via les fonctions gfree et
 * gmalloc.
 *
 * 														Hexanome H4212
 * ********************************************************************/


/* ___________________________________________________________INCLUDES */

#include <stddef.h>
#include <stdio.h>

/* ___________________________________________________________TYPEDEF  */

typedef long Align; /*Permet de garantir l'alignement sur un long*/

union en_tete { /*Bloc d'en-tête*/

    struct {
        union en_tete *ptr; /* Pointe sur le bloc libre suivant */
        unsigned taille; /* Taille du bloc */
    } s;
    
    Align x; /*Force l'alignement sur long*/
};

typedef union en_tete En_tete;

/* ___________________________________________________________DEFINE   */

#define NALLOUE 1024 /* Nombre minimum d'unités allouées lors d'une demande d'agrandissement mémoire */


/*___________________________________________________________STATICS   */

static En_tete base; /* Une liste vide pour commencer */
static En_tete *plibre = NULL; /* Debut de la liste de bloc libre */

/*___________________________________________________________FONCTIONS */

/* Procédure de libération de mémoire (remplace free) */
/* Elle met le bloc en paramètre dans la liste des blocs libres */

void gfree(void *pa) {

    En_tete *pb, *p;

    pb = (En_tete *) pa - 1; /* Pointeur sur l'en-tête */
    for (p = plibre; !(pb > p && pb < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (pb > p || pb < p->s.ptr))
            break; /* le bloc est libéré en début ou en fin */

    if (pb + pb->s.taille == p->s.ptr) { /*Jointure par le haut*/
        pb->s.taille += p->s.ptr->s.taille;
        pb->s.ptr = p->s.ptr->s.ptr;

    } else
        pb->s.ptr = p->s.ptr;

    if (p + p->s.taille == pb) { /*Jointure par le bas*/
        p->s.taille += pb->s.taille;
        p->s.ptr = pb->s.ptr;
    } else
        p->s.ptr = pb;
    plibre = p;
}

/* Procédure d'extension mémoire (opt) */

En_tete *plusmem(unsigned nu) {

    char *pc, *sbrk(int);
    En_tete *pu;

    if (nu < NALLOUE) /* on arrondie à la taille minimum d'agrandissement */
        nu = NALLOUE;
    pc = sbrk(nu * sizeof (En_tete)); /* On effectue l'agrandissement */
    if (pc == (char *) - 1)
        return NULL; /* Cas ou il n'y a plus d'espace ==> echec de sbrk */
    pu = (En_tete *) pc;
    pu->s.taille = nu;
    gfree((void*) (pu + 1));
    return plibre; /* on retourne le nouveau bloc*/
}

/* Procédure d'allocation mémoire (remplace malloc) */

void *gmalloc(unsigned noctets) {

    En_tete *p, *pprec; /* Pointeurs de parcours de la liste */

    unsigned nunites; /*variable stockant le nombre d'unité d'en-tête*/

    nunites = (noctets + sizeof (En_tete) - 1) / sizeof (En_tete) + 1;

    if ((pprec = plibre) == NULL) { /* Il n'existe pas encore de liste ==> On la crée*/
        base.s.ptr = plibre = pprec = &base;
        base.s.taille = 0;
    }
    

    for (p = pprec->s.ptr;; pprec = p, p = p->s.ptr) { /*Parcours de la liste des blocs libre*/
        if (p->s.taille >= nunites) {/* le bloc est assez grand */
            if (p->s.taille == nunites)/* le bloc est tout juste assez grand */
                pprec->s.ptr = p->s.ptr;
            else {                       /* On redimensionne le bloc */
                p->s.taille -= nunites;
                p += p->s.taille;
                p->s.taille = nunites;
            }
            plibre = pprec;
            return (void *) (p + 1); 
        }
        if (p == plibre) /* On a rebouclé la liste ==> il ne reste plus de bloc assez grands */
            if ((p = plusmem(nunites)) == NULL) /* on recupère un gros bloc de mémoire */
                return NULL; 

    }
}










