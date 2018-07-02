/**
 * eratosthenes.h
 * Řešení: IJC-DU1, příklad a), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "bit_array.h"

#ifndef _ERATOSTHENES_H_
#define _ERATOSTHENES_H_

/**
 * Function finds prime indexes of bit array using Eratosthenes algorithm 
 * and sets bits with these indexes to 0.
 * @param pole array of bits
 */
void Eratosthenes(bit_array_t pole);

#endif
