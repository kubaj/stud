/**
 * htable_lookup_add.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>
#include <string.h>

struct htab_listitem *htab_lookup_add(struct htab_t *t, char *key) {

    /// Calculate hash for item
    unsigned hash = t->hash_fun_ptr(key, t->htab_size);
    
    /// Create pointer to first item in list
    struct htab_listitem *item = &t->ptr[hash];

    /// Cycle through whole list
    while(item->key != NULL) {
    
        /// If key is that we search, increase data and n in table, return pointer
        if(strcmp(item->key, key) == 0) {
        
            item->data++;
            t->n++;

            return item;
        }

        /**
         * If key is not that we are searching and there is no item in list 
         * create new item, if allocation wasn't successful, return NULL
         */
        if(item->next == NULL) {
        
            item->next = malloc(sizeof(struct htab_listitem));
            if(item->next == NULL) {
            
                return NULL;
            }

            item->next->key = NULL;
            item->next->next = NULL;
        } 

        /// If new item was added to end of list, while cond. won't be met
        item = item->next;
    }

    /**
     * Alloc. place for new item key, set key to our value, set data to 1 
     * and return pointer
     */

    item->key = malloc(strlen(key) + 1);
    if(item->key == NULL) {
    
        return NULL;
    }

    strcpy(item->key, key);
    item->data = 1;
    t->n++;

    return item;
}

