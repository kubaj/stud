/**
 * steg-decode.c
 * Řešení: IJC-DU1, příklad b), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"
#include "bit_array.h"
#include "eratosthenes.h"
#include "error.h"

int main(int argc, char *argv[]) {

    struct ppm *ppmPicture;
    
    /// Exit program if argument is missing or more arguments have been passed
    if(argc < 2 || argc > 2) { 
    
        fatal_error("Program steg-decode ocakava 1 argument\n");
    }

    /// Read picture fith filename in argument
    ppmPicture = ppm_read(argv[1]);

    /// Exit program if reading of picture failed
    if(ppmPicture == NULL) 
        return -1;

    /**
     * Calculate number of bytes in picture data and create bit_array_t with 
     * this size, then calculate bits in array with prime index
     */
    unsigned long bytesPPM = ppmPicture->xsize*ppmPicture->ysize*3;
    ba_create(primeBytes, bytesPPM);
    Eratosthenes(primeBytes);

    /**
     * Create array to save decoded message, size is (bytes)/2/8, because 
     * all odd number are surely not primes and every byte in picture provides
     * only one bit of data. Set array to zeros.
     */
    unsigned char msgString[bytesPPM/16];
    memset(msgString, 0, bytesPPM/16);

    /// Index of actual bit in array for save
    unsigned long msgChar = 0;

    /** 
     * Iterate through all bytes in picture and save LSB in string if index 
     * of byte array is prime 
     */
    for(unsigned long i = 0; i < bytesPPM; i++) {
    
        if(!(ba_get_bit(primeBytes, i))) {
        
            /// Set bit of final string to value of LSB in picture byte
            // DU1_SET_BIT(msgString, msgChar, (ppmPicture->data[i] & 1));
            msgString[(msgChar)/(CHAR_BIT)] |= ((ppmPicture->data[i] & 1) << (msgChar)%(CHAR_BIT));

            /// After each 8bits = 1 char, check if null byte was read 
            if((msgChar+1)%8 ==0 && msgString[msgChar/8] == 0) {
            
                break;
            }

            msgChar++;
        }
    }

    /// Write decoded message to stdout
    printf("%s\n", msgString);

    // ppm_write(ppmPicture, "out.ppm");

    /// Free memory of structure ppm
    free(ppmPicture);
    
    return 0;

}
