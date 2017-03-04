#include "proj2_sharedres.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

/**
 * Function initialize all needed resources - semaphores, shared memory
 */
void init_res() {

    /// Set all pointers to NULL
    fostream = NULL;
    
    mutex = NULL;
    mutex2 = NULL;
    boardQueue = NULL;
    unboardQueue = NULL;
    allAboard = NULL;
    allAshore = NULL;
    finish = NULL;

    boarders = NULL;
    unboarders = NULL;
    op_number = NULL;

    /// Open output file, write error to stderr if not successful
    fostream = fopen("proj2.out","w");

    if(fostream == NULL) {
    
        fprintf(stderr, "Error: file proj2.out cannot be opened\n");
        exit(1);
    }

    /// Disable buffering of stdout stream
    setvbuf(fostream, NULL, _IONBF, 0);

    /// Open shared memory, print error and exit with code 2 if not successful
    int fildes = shm_open("/xkulic03_shm", O_CREAT | O_TRUNC | O_RDWR, 0666); 
    if(fildes == -1) {
    
        fprintf(stderr, "Error: Shared memory cannot be opened\n");
        free_res();
        exit(2);
    }

    /// Truncate file size for 3 int numbers
    ftruncate(fildes, 3*sizeof(int));

    /// Map shared memory to addresses
    boarders = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, fildes, 0);
    unboarders = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, fildes, 0);
    op_number = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, fildes, 0);

    /// Close file descriptor
    close(fildes);

    /// If mapping of memory wasn't successful, exit with code 2
    if(boarders == NULL || unboarders == NULL || op_number == NULL) {
    
        fprintf(stderr, "Error: Mapping of shared memory failed\n");
        free_res();
        exit(2);
    }

    /// Set variables to needed value
    *boarders = 0;
    *unboarders = 0;
    *op_number = 1;

    /// Initialize all semaphores
    if((mutex = sem_open("/xkulic03_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) sem_error();
    if((mutex2 = sem_open("/xkulic03_mutex2", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) sem_error();
    if((boardQueue = sem_open("/xkulic03_boardQueue", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) sem_error();
    if((unboardQueue = sem_open("/xkulic03_unboardQueue", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) sem_error();
    if((allAboard = sem_open("/xkulic03_allAboard", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) sem_error();
    if((allAshore = sem_open("/xkulic03_allAshore", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) sem_error();
    if((finish = sem_open("/xkulic03_finish", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) sem_error();
}

/**
 * Function free all resources allocated in init_res()
 */
void free_res() {

    /// Close filestream if opened
    if(fostream != NULL) {
    
        fclose(fostream);
    }

    /// Unmap shared memory if mapped

    if(boarders != NULL) {
    
        munmap(boarders, sizeof(int));
    }

    if(unboarders != NULL) {
    
        munmap(unboarders, sizeof(int));
    }

    if(op_number != NULL) {
    
        munmap(op_number, sizeof(int));
    }

    /// Unlink shared memory
    shm_unlink("/xkulic03_shm");

    /// Close all semaphores if opened
    if(mutex != NULL) {
    
        sem_close(mutex);
        sem_unlink("/xkulic03_mutex");
    }

    if(mutex2 != NULL) {
    
        sem_close(mutex2);
        sem_unlink("/xkulic03_mutex2");
    }

    if(boardQueue != NULL) {
    
        sem_close(boardQueue);
        sem_unlink("/xkulic03_boardQueue");
    }

    if(unboardQueue != NULL) {
    
        sem_close(unboardQueue);
        sem_unlink("/xkulic03_unboardQueue");
    }

    if(allAboard != NULL) {
    
        sem_close(allAboard);
        sem_unlink("/xkulic03_allAboard");
    }

    if(allAshore != NULL) {

        sem_close(allAshore);
        sem_unlink("/xkulic03_allAshore");
    }

    if(finish != NULL) {

        sem_close(finish);
        sem_unlink("/xkulic03_finish");
    }
}

/**
 * Function is used while initializing semaphore wasn't successful.
 * Function prints error, free resources and exit with error code 2
 */ 
void sem_error() {

    fprintf(stderr, "Error: Semaphore cannot be opened\n");
    free_res();
    exit(2);
}
