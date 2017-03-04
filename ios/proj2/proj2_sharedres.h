#include <semaphore.h>
#include <stdio.h>

#ifndef _PROJ2_SHAREDRES_H_
#define _PROJ2_SHAREDRES_H_

FILE *fostream; /// Pointer to output stream

sem_t *mutex;
sem_t *mutex2;
sem_t *boardQueue;
sem_t *unboardQueue;
sem_t *allAboard;
sem_t *allAshore;
sem_t *finish;

unsigned *boarders;   /// Number of passengers that already boarded into car
unsigned *unboarders; /// Number of passengers that already unboarded from car
unsigned *op_number;  /// Number of operation - used in printing status of processes

unsigned param_p;  /// Number of passengers
unsigned param_c;  /// Capacity of car
unsigned param_pt; /// Maximum time before creating new passenger
unsigned param_rt; /// Maximum time that car runs

/**
 * Function initialize all needed resources - semaphores, shared memory
 */
void init_res();

/**
 * Function free all resources allocated in init_res()
 */
void free_res();

/**
 * Function is used while initializing semaphore wasn't successful.
 * Function prints error, free resources and exit with error code 2
 */ 
void sem_error();

#endif
