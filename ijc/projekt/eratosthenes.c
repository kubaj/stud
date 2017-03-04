/**
 * eratosthenes.c
 * Řešení: IJC-DU1, příklad a), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "bit_array.h"
#include <stdio.h>
#include <math.h>

void Eratosthenes(bit_array_t pole) {
    
    /** 
     * Numbers 0 and 1 are not primes and number 2 is prime.
     */
    if(ba_size(pole) > 0) {
    
        ba_set_bit(pole, 0, 1);
    }
    if(ba_size(pole) > 1) {
    
        ba_set_bit(pole, 1, 1);
    }
    if(ba_size(pole) > 2) {
    
        ba_set_bit(pole, 2, 0);
    }

    /**
     * Set bits with even index to 1 - these are'nt prime.
     * Set bits with odd index to 0 - we don't know which of these are prime.
     */ 
    if(ba_size(pole) > 3) {
    
        for(unsigned long i = 3; i < ba_size(pole); i++) {
        
            if(i%2 == 0) ba_set_bit(pole, i, 1);
            else    ba_set_bit(pole, i, 0);

        }
    }

    /**
     * Set all multiplies of the least prime number to 1. Then find next
     * prime number and repeat setting of multiplies to 1. Iterate through 
     * all primes numbers that are <= than size of array.
     */
    if(ba_size(pole) > 3) {

        for(unsigned long i = 3; i <= sqrt(ba_size(pole)); i+=2) {
        
            if(!(ba_get_bit(pole, i))) {

                unsigned long j = 2*i;
                while(j < ba_size(pole)) {
                
                    if(j < i) break;
                    ba_set_bit(pole, j, 1);
                    j+=i;
                }
            }
        }
    }
}
