/**
 * htable_hash_function.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"

unsigned int hash_function(const char *str, unsigned htab_size) {

    unsigned int h=0;
    const unsigned char *p;
    
    for(p=(const unsigned char*)str; *p!='\0'; p++) {
    
        h = 65599*h + *p;
    }

    return h % htab_size;
}
