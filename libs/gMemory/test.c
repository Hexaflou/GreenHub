#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "gMemory.h"

/*Programme de test de la libraire gMemory*/


/*Définition de la liste chainée*/

typedef struct element element;
struct element
{
    int val;
    struct element *nxt;
};
 
typedef element* llist;
 
/*fonction d'ajout en tête*/

 llist ajouterEnTete(llist liste, int valeur)
{
    
    element* nouvelElement = gmalloc(sizeof(element));
 
    
    nouvelElement->val = valeur;
 
    
    nouvelElement->nxt = liste;
 
    
    return nouvelElement;
}

/*fonction d'ajout en fin*/

llist ajouterEnFin(llist liste, int valeur)
{
    
    element* nouvelElement = gmalloc(sizeof(element));
 
    
    nouvelElement->val = valeur;
 
    
    nouvelElement->nxt = NULL;
 
    if(liste == NULL)
    {
        
        return nouvelElement;
    }
    else
    {
        
        
        element* temp=liste;
        while(temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}
 
 /*fonction d'affichage*/
 
 void afficherListe(llist liste)
{
    element *tmp = liste;
    
    while(tmp != NULL)
    {
        
        printf("%d ", tmp->val);
        
        tmp = tmp->nxt;
    }
}

/*fonction de désallocation*/

llist effacerListe(llist liste)
{
    element* tmp = liste;
    element* tmpnxt;
 
    
    while(tmp != NULL)
    {
        
        tmpnxt = tmp->nxt;
        
        gfree(tmp);
        
        tmp = tmpnxt;
    }
    
    return NULL;
}



int main() {
   
	llist ma_liste = NULL; /*Initialisation de la liste*/
    int i=1; /*Variable compteur*/
    char* doubleGmalloc = NULL; /*Pointeurs de test*/
    char * pointeur = NULL
   
    printf("\nTest d'allocation simple\n\n");
   
    pointeur = gmalloc(20 * sizeof (char)); 
    
    if (pointeur == NULL) 
	{
        printf("L'allocation n'a pu être réalisée\n");
    } 
    else 
    {
        printf("L'allocation a été un succès\n");
        gfree(pointeur);
    }
    
    printf("\nTest d'allocation liste chainée \n\n");
    

    for(i=1;i<=1000;i++)
    {
        ma_liste = ajouterEnTete(ma_liste, i);
        ma_liste = ajouterEnFin(ma_liste, i);
    }
   
    
    printf("\nOn s'attend à l'affichage de 1000 valeurs croissantes puis 1000 décroissantes\n\n");
    
	afficherListe(ma_liste);
	effacerListe(ma_liste);
  
	printf("\nDouble gmalloc\n\n");
 
	doubleGmalloc=gmalloc(20 * sizeof (char));
	doubleGmalloc="Test malloc 1 \n";
	
	printf("Après gmalloc 1 : pointeur : 0x%x , Valeur : %s\n",*doubleGmalloc,doubleGmalloc);
	
	doubleGmalloc=gmalloc(20 * sizeof (char));
	doubleGmalloc="Test malloc 2 \n";
	
	printf("Après gmalloc 2 : pointeur : 0x%x , Valeur : %s\n",*doubleGmalloc,doubleGmalloc);
 
 
	printf("\nDouble gfree\n\n On s'attend à une segmentation fault !\n\n");
    
    free(doubleGmalloc);
    free(doubleGmalloc);
    
    
    return 0;
}
