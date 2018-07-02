/**
 * wordcount.c
 * Řešení: IJC-DU2, příklad b), 08.04.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "htable.h"

/**
 * Function prints string "key" and value of "value" delimited by tabulator
 * @param *key Pointer to array of characters to print
 * @param value Number to print
 */
void printItem(char *key, int value);

int main() {

    /**
     * Determination of size was based on number of english words (~1 million)
     *
     * Size of table should be prime. This helps to better distribution of words
     * in hash table. Also, size should be in the middle between two powers of two.
     */
    struct htab_t *htable;
    htable = htab_init(1572869);

    /// If table wasn't allocated successfuly, end program
    if(htable == NULL) {
    
        fprintf(stderr, "Error: Table couldn't be created\n");

        return 1;
    }

    char buffer[127];
    int readStatus;
    
    /// Read all words from stdin and add them to hash table
    while((readStatus = get_word(buffer, 127, stdin)) != EOF) {
    
        /// If adding file to table wasn't successful, free allocated resources and end program.
        if(htab_lookup_add(htable, buffer) == NULL) {
        
            fprintf(stderr, "Error: Word \"%s\" couldn't be added to table\n", buffer);
            htab_free(htable);
            
            return 2;
        }
    }

    /// Call function "printItem" for each item in hash table 
    htab_foreach(htable, printItem);

    /// Free hash table
    htab_free(htable);

    return 0;
}

/// Foreach function to print all values in hash table
void printItem(char *key, int value) {

    printf("%s\t%u\n", key, value);
}

