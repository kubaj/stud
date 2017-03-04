/**
 * io.h
 * Řešení: IJC-DU2, příklad b), 08.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <stdio.h> 

#ifndef _IO_H_
#define _IO_H_

/**
 * Function load one word separated by white-space character(space,'\n','\f',
 * '\r','\t','\v' from input f with maximum length of max-1 and saves it into
 * array of characters s
 * @param *s Pointer to array, where word is saved
 * @param max Maximum number of characters that can be saved into array
 * @param *f Pointer to input stream 
 * @return Number of characters loaded from stream
 */
int get_word(char *s, int max, FILE *f);

#endif
