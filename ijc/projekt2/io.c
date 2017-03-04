/**
 * io.c
 * Řešení: IJC-DU2, příklad b), 08.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "io.h"
#include <stdio.h>
#include <ctype.h>

int get_word(char *s, int max, FILE *f) {

    /// Number of characters already loaded
    int loaded = 0;

    /// Buffer for loading characters
    int ch;

    /// Ensure warning message is printed only once
    int errorPrinted = 0;

    /// Load only max-1 characters, last byte reserved for '\0'
    max -= 1;

    while(1) {
    
        /// Load one character from input stream f
        ch = fgetc(f);

        if(ch == EOF) {
        
            return EOF;

        } else if(isspace(ch)) {
        
            if(loaded) {

                break;
            }

        } else if(loaded < max) {
        
            s[loaded] = ch;

            loaded++;

        } else if(!errorPrinted) {
        
            fprintf(stderr,"Warning: input word longer than buffer\n");
            errorPrinted = 1;
        }
    }

    /// Set last byte to '\0'
    s[loaded] = 0;

    /// Return number of loaded characters
    return loaded;
}
