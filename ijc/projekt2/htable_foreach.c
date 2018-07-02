/**
 * htable_foreach.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include "stdlib.h"

void htab_foreach(struct htab_t *t, void (*func)(char *, int)) {

    struct htab_listitem *item = NULL;

    /// Iterate through all lists and then iterate through each one list and call func.
    for(unsigned i = 0; i < t->htab_size; i++) {
    
        item = &t->ptr[i];

        while(item != NULL && item->key != NULL) {
        
            func(item->key, item->data);
            item = item->next;
        }
    }
}
