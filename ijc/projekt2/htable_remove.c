/**
 * htable_remove.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>
#include <string.h>

void htab_remove(struct htab_t *t, char *key) { 

    unsigned hash = t->hash_fun_ptr(key, t->htab_size);
    
    struct htab_listitem *item = &t->ptr[hash];
    struct htab_listitem *item2 = &t->ptr[hash];

    /**
     * We can't free memory of first item in list, so if searched item is first
     * in list we copy data from last item and remove last item.
     *
     * If searched item isn't first in list, we copy pointer of next item to 
     * previous item and then we remove item.
     *
     * Removing item consists from freeing memory of key and then freeing 
     * memory of item.
     */
    if(item->key != NULL && strcmp(item->key, key) == 0) {
    
        while(item2->next->next == NULL) {

            item2 = item2->next;
        }
        free(item->key);
        item->key = malloc(strlen(item2->next->key));
        strcpy(item->key, item2->next->key);
        item->data = item2->next->data;
        free(item2->next->key);
        free(item2->next);
        item2->next = NULL;

    } else if(item->next != NULL){
    
        do {

            if(item->next->key != NULL && strcmp(item->next->key, key) == 0) {
            
                item2 = item->next->next;
                free(item->next->key);
                free(item->next);
                item->next = item2;
            }
            item = item->next;

        } while(item->next->next != NULL);
    }

    /// Decrease number of entries in hash table 
    t->n--;
}
