/**
 * htable_free.c
 * Řešení: IJC-DU2, příklad b), 10.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "htable.h"
#include <stdlib.h>

void htab_free(struct htab_t *t) {

    /// Same as htab_clear but hash table is also freed
    htab_clear(t);
    free(t);
}
