/**
 * htable_init.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>

struct htab_t *htab_init(unsigned size) {

    struct htab_t *hash_table;
    hash_table = malloc(sizeof(struct htab_t) + size*sizeof(struct htab_listitem));

    if(hash_table == NULL) {
    
        return NULL;
    }

    hash_table->htab_size = size;
    hash_table->n = 0;
    hash_table->hash_fun_ptr = hash_function;

    for(unsigned i = 0; i < hash_table->htab_size; i++) {
    
        hash_table->ptr[i].key = NULL;
        hash_table->ptr[i].next = NULL;
        hash_table->ptr[i].data = 0;
    }

    return hash_table;
}
