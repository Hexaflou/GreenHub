/*
 * Utility.c
 *
 *  Created on: 18 janv. 2012
 *      Author: H4212
 */

#include "Utility.h"

/*

Copyright(c) 2007. Victor M. Alvarez [plusvic@gmail.com].

This function was found on
	http://code.google.com/p/yara-project/source/browse/trunk/libyara/xtoi.c?r=74
	by Nicolas Phan

*/
unsigned int xtoi(const char* hexstr)
{
        unsigned int r = 0;
        int i;
        int l = strlen(hexstr);

        for (i = 0; i < l; i++)
        {
                switch(hexstr[i])
                {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                                r |= (hexstr[i] - '0') << ((l - i - 1) * 4);
                                break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                                r |= (hexstr[i] - 'a' + 10) << ((l - i - 1) * 4);
                                break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                                r |= (hexstr[i] - 'A' + 10) << ((l - i - 1) * 4);
                                break;
                        default:
                                i = l;  /* force loop exit */
                }
        }

        return r;
}


/* This function took a leaf out of a function found on
 * 	http://nicolasj.developpez.com/articles/libc/string/#LIII-C
 * 	by Nicolas Phan
 * It extracts an array of char of an array of char.
 * Input :
 * 	s : constant char *, the array of char which result will be extracted from.
 * 	start : unsigned int, start's index of the extraction
 * 	end : unsigned int, end's index of the extraction
 */
char *str_sub (const char *s, unsigned int start, unsigned int end)
{
   char *new_s = NULL;

   if (s != NULL && start < end)
   {
      new_s = malloc (sizeof (*new_s) * (end - start + 2));
      if (new_s != NULL)
      {
         int i;

         for (i = start; i <= end; i++)
         {
            new_s[i-start] = s[i];
         }
         new_s[i-start] = '\0';
      }
      else
      {
         fprintf (stderr, "Insufficient memory.\n");
         exit (EXIT_FAILURE);
      }
   }
   return new_s;
}

