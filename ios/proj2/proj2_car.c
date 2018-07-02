#include "proj2_car.h"
#include "proj2_sharedres.h"
#include "proj2_passenger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Function represents car. Function starts car using function car_start(). 
 * Function calls functions to load passengers, run car, unload passengers 
 * while all passengers are not transited. Functions then finish car using 
 * function car_finish().
 * @param car_id Identification number of car
 */
void proj2_car(unsigned car_id) {

    /// Number of cycles that will car made
    unsigned car_rides = param_p/param_c;

    /// Run car
    car_start(car_id);

    /// Make car load passengers, run and then unload passengers
    while(car_rides) {

        car_load(car_id);
        car_run(car_id);
        car_unload(car_id);

        car_rides--;
    }
    
    /// Finish car
    car_finish(car_id);

    exit(0);
}

/**
 * Function starts car. 
 * @param car_id Identification number of car
 */
void car_start(unsigned car_id) {

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: C %u\t: started\n", (*op_number)++, car_id);
    
    /// Allow others processes to print
    sem_post(mutex2);
}

/**
 * Function loads passengers into car.
 * @param car_id Identification number of car
 */
void car_load(unsigned car_id) {

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: C %u\t: load\n", (*op_number)++, car_id);
    
    /// Allow others processes to print
    sem_post(mutex2);

    /// Set semaphore boardQueue to capacity of car, so passengers can board
    for(unsigned i = 0; i < param_c; i++) sem_post(boardQueue);

    /// Wait until all passengers are aboard
    sem_wait(allAboard);
}

/**
 * Function runs car.
 * @param car_id Identification number of car
 */
void car_run(unsigned car_id) {

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: C %u\t: run\n", (*op_number)++, car_id);

    /// Allow others processes to print
    sem_post(mutex2);

    if(param_rt > 0) {
    
        usleep(random()%param_rt*1000);
    }
}

/**
 * Function unloads passengers from car.
 * @param car_id Identification number of car
 */
void car_unload(unsigned car_id) {

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: C %u\t: unload\n", (*op_number)++, car_id);

    /// Allow others processes to print
    sem_post(mutex2);

    /// Set semaphore unboardQueue to capacity of car, so passengers can unboard
    for(unsigned i = 0; i < param_c; i++) sem_post(unboardQueue);

    /// Wait until car is empty
    sem_wait(allAshore);
}

/**
 * Function finishes car.
 * @param car_id Identification number of car
 */
void car_finish(unsigned car_id) {

    for(unsigned i = 0; i < param_p; i++) sem_post(finish);

    /// Prevent others processes to print
    sem_wait(mutex2);
    fprintf(fostream, "%u\t: C %u\t: finished\n", (*op_number)++, car_id);

    /// Allow others processes to print
    sem_post(mutex2);
}
