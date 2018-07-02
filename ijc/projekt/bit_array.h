/**
 * bit_array.h
 * Řešení: IJC-DU1, příklad a), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <limits.h>
#include <string.h>
#include "error.h"

#ifndef _BITARRAY_H
#define _BITARRAY_H

typedef unsigned long bit_array_t[];

#define ba_create(arr, size) \
     unsigned long arr [(CHAR_BIT*sizeof(long) - 1 + size)/(CHAR_BIT*sizeof(long)) + 1]; \
     memset(arr, 0, (CHAR_BIT*sizeof(long) - 1 + size)/(CHAR_BIT*sizeof(long)) + 1); \
     arr[0] = size;

#define DU1_GET_BIT_(arr, index) 1 & (arr[(index)/(CHAR_BIT * sizeof(arr[0]))] >> ((index)%(CHAR_BIT*sizeof(arr[0]))))

#define DU1_SET_BIT_(arr, index, bit) arr[(index)/(CHAR_BIT * sizeof(arr[0]))] |= ((unsigned long)bit << (index)%(CHAR_BIT*sizeof(arr[0])))

/**
 * Implementation of inline functions
 */
#ifdef USE_INLINE

static inline unsigned long ba_size(bit_array_t arr) {

     return arr[0];
}

static inline char ba_get_bit(bit_array_t arr, unsigned long index) {

    if(index < ba_size(arr)) {
    
        return (DU1_GET_BIT_(arr,index + CHAR_BIT*sizeof(long)));

    } else {
    
        fatal_error("Index %lu mimo rozsah 0..%lu", index, ba_size(arr));
        return 0;
    }

}


static inline void ba_set_bit(bit_array_t arr, unsigned long index, unsigned long bit) {

    if(index < ba_size(arr)) {
    
        DU1_SET_BIT_(arr,index + CHAR_BIT*sizeof(long),bit);

    } else {
    
        fatal_error("Index %lu mimo rozsah 0..%lu", index, ba_size(arr));
    }

}

#else

/**
 * Implementation of macros
 */

#define ba_size(arr) arr[0]

#define ba_get_bit(arr, index) (index < arr[0]) ? DU1_GET_BIT_(arr, index + CHAR_BIT*sizeof(long)) : (fatal_error("Index %lu mimo rozsah 0..%lu", index, ba_size(arr)), 42)

#define ba_set_bit(arr, index, bit) (index < arr[0]) ? DU1_SET_BIT_(arr, index + CHAR_BIT*sizeof(long), bit) : (fatal_error("Index %lu mimo rozsah 0..%lu", index, ba_size(arr)), 42)

#endif

#endif
