#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "gMemory.h"

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

int main(int argc, char *argv[]) {
    char * pointeur = gmalloc(20 * sizeof (char)); /* Allocation de 20 octets */
    if (pointeur == NULL) {
        printf("L'allocation n'a pu être réalisée\n");
    } else {
        printf("L'allocation a été un succès\n");
        gfree(pointeur); /* Libération des 20 octets précédemment alloués */
    }
    return 0;
}
