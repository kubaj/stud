/**
 * primes.c
 * Řešení: IJC-DU1, příklad a), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"
#include "eratosthenes.h"

int main() {

    /// Declare and initialize array of bits
    ba_create(arrayBits,202000000);

    /// Find all bits with prime index
    Eratosthenes(arrayBits);

    /// Array of last ten prime numbers 
    unsigned long lastPrimes[10];

    /// Find last ten primes and print them to stdout 
    int x = 0;
    for(unsigned long i = ba_size(arrayBits) - 1; i > 0 && x < 10; i--) {
    
        if((ba_get_bit(arrayBits, i)) == 0) {
            
            lastPrimes[x] = i;
            x++;

        }
    }

    for(int i = 9; i >= 0; i--) printf("%lu\n", lastPrimes[i]);

    return 0;

}
