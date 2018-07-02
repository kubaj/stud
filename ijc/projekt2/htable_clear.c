/**
 * htable_clear.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>

void htab_clear(struct htab_t *t) {

    struct htab_listitem *item = NULL;
    struct htab_listitem *itemNext = NULL;

    /**
     * Removing items works as in htab_remove, in cycle remove always second 
     * item from list while list consists only from 1 item.
     * Then free key in first item, if was allocated, pointers to NULL, data to 0
     * and set hash table number of entries to 0
     */

    for(unsigned i = 0; i < t->htab_size; i++) {
    
        item = &t->ptr[i];

        while(item->next != NULL) {
        
            if(item->next->key != NULL) {
            
                free(item->next->key);
            }
            
            itemNext = item->next->next;

            free(item->next);
            item->next = itemNext;
        }

        if(item->key != NULL) {
        
            free(item->key);
        }

        item->next = NULL;
        item->key = NULL;
        item->data = 0;

    }

    t->n = 0;
}
