#ifndef UTILITY_H_
#define UTILITY_H_

/***********************************************************************
 *				Utility.h
 *
 * Interface proposant plusieurs fonctions utiles au fonctionnement
 * de notre application.
 *
 * 							Hexanome H4212
 * ********************************************************************/

/* Inclusions externes */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

/* Fonction convertissant une chaîne de caractère correspondant à un nombre hexadécimal en entier */
unsigned int xtoi(const char* hexstr);

/* Renvoie une chaîne de caractère correspondant à une sous-chaîne de caractère de s */
char *str_sub(const char *s, unsigned int start, unsigned int end);
#endif /* UTILITY_H_ */
