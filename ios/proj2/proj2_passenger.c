#include "proj2_passenger.h"
#include "proj2_sharedres.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Function represents passenger. Function starts passenger using function 
 * passenger_start(). 
 * Function calls functions to board, unboard. Functions then finish passenger 
 * using function passenger_finish().
 * @param passen_id Identification number of car
 */
void proj2_passenger(unsigned passen_id) {

    passenger_start(passen_id);
    passenger_board(passen_id);
    passenger_unboard(passen_id);
    passenger_finish(passen_id);

    exit(0);
}

/**
 * Function starts passenger. 
 * @param passen_id Identification number of car
 */
void passenger_start(unsigned passen_id) {

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: P %u\t: started\n", (*op_number)++, passen_id);

    /// Allow others processes to print
    sem_post(mutex2);
}

/**
 * Function boards passenger into car.
 * @param passen_id Identification number of car
 */
void passenger_board(unsigned passen_id) {

    /// Wait for boarding queue to open
    sem_wait(boardQueue);

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: P %u\t: board\n", (*op_number)++, passen_id);

    /// Allow others processes to print
    sem_post(mutex2);
    
    /// Prevent others processes to access "boarders"
    sem_wait(mutex);
    
    /// Prevent others processes to print
    sem_wait(mutex2);

    /**
     * Increment number of boarders. If number of boarders is equal to capacity 
     * of car then close queue to car
     */

    *boarders += 1;

    fprintf(fostream, "%u\t: P %u\t: board ", (*op_number)++, passen_id);

    if(*boarders == param_c) {

        fprintf(fostream, "last\n");
        sem_post(allAboard);
        *boarders = 0;

    } else {

        fprintf(fostream, "order %u\n", (*boarders));
    }

    /// Allow others processes to print
    sem_post(mutex2);
    
    /// Allow others processes to access "boarders"
    sem_post(mutex);
}

/**
 * Function unboards passenger into car.
 * @param passen_id Identification number of car
 */
void passenger_unboard(unsigned passen_id) {

    sem_wait(unboardQueue);

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: P %u\t: unboard\n", (*op_number)++, passen_id);

    /// Allow others processes to print
    sem_post(mutex2);

    /// Prevent others processes to access "boarders"
    sem_wait(mutex);

    /// Prevent others processes to print
    sem_wait(mutex2);

    /**
     * Increment number of unboarders. If number of unboarders is equal to capacity 
     * of car then allow car to finish unloading.
     */

    *unboarders += 1;

    fprintf(fostream, "%u\t: P %u\t: unboard ", (*op_number)++, passen_id);

    if(*unboarders == param_c) {
    
        fprintf(fostream, "last\n");
        sem_post(allAshore);
        *unboarders = 0;

    } else {
    
        fprintf(fostream, "order %u\n", (*unboarders));
    }

    /// Allow others processes to print
    sem_post(mutex2);
    
    /// Allow others processes to access "boarders"
    sem_post(mutex);
}

/**
 * Function finishes passenger. 
 * @param passen_id Identification number of car
 */
void passenger_finish(unsigned passen_id) {

    /// Wait for all cars and passengers to finish unboarding
    sem_wait(finish);

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: P %u\t: finished\n", (*op_number)++, passen_id);

    /// Allow others processes to print
    sem_post(mutex2);
}
