#ifndef _PROJ2_CAR_H_
#define _PROJ2_CAR_H_

/**
 * Function represents car. Function starts car using function car_start(). 
 * Function calls functions to load passengers, run car, unload passengers 
 * while all passengers are not transited. Functions then finish car using 
 * function car_finish().
 * @param car_id Identification number of car
 */
void proj2_car(unsigned car_id);

/**
 * Function starts car. 
 * @param car_id Identification number of car
 */
void car_start(unsigned car_id);

/**
 * Function loads passengers into car.
 * @param car_id Identification number of car
 */
void car_load(unsigned car_id);

/**
 * Function runs car.
 * @param car_id Identification number of car
 */
void car_run(unsigned car_id);

/**
 * Function unloads passengers from car.
 * @param car_id Identification number of car
 */
void car_unload(unsigned car_id);

/**
 * Function finishes car.
 * @param car_id Identification number of car
 */
void car_finish(unsigned car_id);

#endif
