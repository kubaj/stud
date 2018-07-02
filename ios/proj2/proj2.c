#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <assert.h>
#include <limits.h>

#include "proj2_sharedres.h"
#include "proj2_car.h"
#include "proj2_passenger.h"

pid_t parentPid;

void sigquit_handler(int sig);
void killChildrenExit();
void parseArguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    /// Parse all arguments to variables
    parseArguments(argc, argv);

    /// Set handling function for signal SIGQUIT
    signal(SIGQUIT, sigquit_handler);
    /// Store pid of parent proccess
    parentPid = getpid();
    
    /// Initialize all semaphores and shared memory
    init_res();

    /// Create new process for car
    pid_t carPid = fork();

    if (carPid == 0) {

        ///
        /// Run part dedicated for car
        ///
        
        proj2_car(1);

    } else if (carPid == -1) {  

        /// If forking wasn't successful, print error and exit program.
        fprintf(stderr, "Error: Car process couldn't be created\n");
        killChildrenExit();
    }

    /// Create new auxiliary process. Auxiliary process is creating passenger processes
    pid_t auxPid = fork();

    if (auxPid == 0) {

        ///
        /// Code dedicated for auxiliary process starts here
        ///
        
        /// Pid for new generated processes
        pid_t pasPid;
        
        /// ID of car. Each car has unique ID from interval 0 - P
        unsigned passen_id = 1;

        /// Generate P passengers
        while(passen_id <= param_p) {

            /// Wait random time before generating new passenger process
            if(param_pt > 0) {
            
                usleep(random()%param_pt*1000);
            }

            pasPid = fork();

            if(pasPid == 0) {

                break;

            } else if (pasPid == -1) {
            
                /// If forking wasn't successful, print error and exit program.
                fprintf(stderr, "Error: Passenger process couldn't be created\n");
                killChildrenExit();
            }

            passen_id++;
        }

        if(pasPid == 0) {

            ///
            /// Code dedicated for passenger processes starts here
            ///
            
            proj2_passenger(passen_id);
        }

        /// Wait until all passenger processes end and then exit auxiliary process
        wait(NULL);
        exit(0);

    } else if (auxPid == -1) {  

        /// If forking wasn't successful, print error and exit program.
        fprintf(stderr, "Error: Auxiliary process couldn't be created\n");
        killChildrenExit();
    } 

    /// Wait until car and auxiliary process end, then free resources and exit
    wait(NULL);

    free_res();

    return 0;
}

/**
 * Function that handles SIGQUIT signal. If signal is catched, all processes
 * in tree that don't have pid same as parent process are exited.
 */
void sigquit_handler(int sig) {

    assert(sig == SIGQUIT);

    /// Get PID of process and exit process if PID is not as pid of parent
    pid_t self = getpid();
    if(parentPid != self) _exit(0);
}

/**
 * Function sends signal to kill all children, free resources using free_res() 
 * and exits program with error code 2.
 */
void killChildrenExit() {

    /// Send SIGQUIT signal
    kill(-parentPid, SIGQUIT);

    /// Wait for all child processes to finish
    wait(NULL);

    /// Free resources and exit
    free_res();
    exit(2);
}

/**
 * Function parses and checks program arguments. If arguments are wrong, program
 * exits with error code 1
 * @param argc Number of arguments
 * @param argv Pointer to array of arguments
 */
void parseArguments(int argc, char *argv[]) {

    /// 4 arguments are expected: P, C, PT, RT
    if(argc < 5 || argc > 5) {
    
        fprintf(stderr, "Error: Program expects 4 arguments\n");
        exit(1);
    }

    /// *end is used for error checking in strtol
    char *end = NULL;

    /// Variable to temporary save INT values from strings
    long argumentValue = 0;

    /**
     * Each value is parsed using function strtol. After successful parsing,
     * strtol saves pointer to nullbyte in variable "end".
     * If value is out of range or string is not number, print error and exit
     * with code 1. 
     *
     * Range of values:
     * P > 0
     * C > 0
     * 0 <= PT < 5001
     * 0 <= RT < 5001
     */

    argumentValue = strtol(argv[1], &end, 10);
    if(*end == 0 && argumentValue <= UINT_MAX
            && argumentValue > 0) {

        param_p = argumentValue;

    } else {

        fprintf(stderr, "Error: Argument P is not valid\n");
        exit(1);
    } 

    argumentValue = strtol(argv[2], &end, 10);
    if(*end == 0 && argumentValue <= UINT_MAX
            && argumentValue > 0) {

        param_c = argumentValue;

    } else {

        fprintf(stderr, "Error: Argument C is not valid\n");
        exit(1);
    } 

    argumentValue = strtol(argv[3], &end, 10);
    if(*end == 0 && argumentValue <= 5000
            && argumentValue >= 0) {

        param_pt = argumentValue;

    } else {

        fprintf(stderr, "Error: Argument PT not valid\n");
        exit(1);
    } 

    argumentValue = strtol(argv[4], &end, 10);
    if(*end == 0 && argumentValue <= 5000
            && argumentValue >= 0) {

        param_rt = argumentValue;

    } else {

        fprintf(stderr, "Error: Argument RT not valid\n");
        exit(1);
    } 

    /**
     * Check of other conditions:
     * P is multiply of C
     * P > C
     */
    if(param_p % param_c != 0) {
    
        fprintf(stderr, "Error: P value must be multiply of C value\n");
        exit(1);
    }

    if(param_p < param_c) {
    
        fprintf(stderr, "Error: P value must be greater than C value\n");
        exit(1);
    }
}
