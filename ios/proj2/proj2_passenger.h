#ifndef _PROJ2_PASSENGER_H_
#define _PROJ2_PASSENGER_H_

/**
 * Function represents passenger. Function starts passenger using function 
 * passenger_start(). 
 * Function calls functions to board, unboard. Functions then finish passenger 
 * using function passenger_finish().
 * @param passen_id Identification number of car
 */
void proj2_passenger(unsigned passen_id);

/**
 * Function starts passenger. 
 * @param passen_id Identification number of car
 */
void passenger_start(unsigned passen_id);

/**
 * Function boards passenger into car.
 * @param passen_id Identification number of car
 */
void passenger_board(unsigned passen_id);

/**
 * Function unboards passenger into car.
 * @param passen_id Identification number of car
 */
void passenger_unboard(unsigned passen_id);

/**
 * Function finishes passenger. 
 * @param passen_id Identification number of car
 */
void passenger_finish(unsigned passen_id);
#endif
