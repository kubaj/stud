/**
 * htable_init2.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>

struct htab_t *htab_init2(unsigned size, unsigned (*hashfn)(const char *, unsigned)) {

    struct htab_t *hash_table;
    hash_table = htab_init(size);

    if(hash_table == NULL) {
    
        return NULL;
    }

    hash_table->hash_fun_ptr = hashfn;

    return hash_table;
}
