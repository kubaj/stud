/**
 * ppm.c
 * Řešení: IJC-DU1, příklad b), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"
#include "error.h"

struct ppm *ppm_read(const char *filename) {

    /// Values from file header are loaded to these 3 variables
    unsigned int size_x;
    unsigned int size_y;
    unsigned int bitdepth;

    /// Pointer to structure ppm, where image is loaded
    struct ppm *ppm_picture = NULL;

    
    FILE *file = NULL;
    file = fopen(filename, "r");

    /// File cannot be opened 
    if(file == NULL) 
        warning_msg("Subor %s nie je mozne otvorit.\n", filename);

    /// Variable to save first line in header - Magic number P6
    char meta[11];
    
    if(file != NULL) {
    
        /// Read first line and check if 'P6' is in header
        fscanf(file, " %10s\n", meta);

        if(strcmp(meta,"P6")) {

            fclose(file);
            file = NULL;
            warning_msg("Subor %s je poskodeny. Subor nie je mozne otvorit.\n", filename);
        }
    }
    
    /// Return value of scanning functions
    int scanStatus;

    if(file != NULL) {

        /**
         * Read 2nd line - width and height of a picture, check size of picture
         * and set file to NULL if reading wasn't succesful
         */
        scanStatus = fscanf(file, "%u %u\n", &size_x, &size_y);

        if(scanStatus != 2 || size_x < 1 || size_y < 1 
                || size_x > 5000 || size_y > 5000) {
        
            fclose(file);
            file = NULL;
            warning_msg("Maximalna velkost suboru je 5000x5000.\n \
                    Subor %s nie je mozne otvorit.\n", filename);
        }
    }

    if(file != NULL) {

        /// Read 3rd line where bitdepth of picture is
        scanStatus = fscanf(file, "%u\n", &bitdepth);

        if(scanStatus != 1 || bitdepth != 255) {
        
            fclose(file);
            file = NULL;
            warning_msg("Subor %s je poskodeny. Subor nie je mozne otvorit.\n", filename);
        }
    }

    if(file != NULL) {
        
        /** 
         * If reading of all atributes was succesful, allocate size for
         * picture data
         */
        ppm_picture = malloc(sizeof(struct ppm) + size_x * size_y * 3);
        ppm_picture->xsize = size_x;
        ppm_picture->ysize = size_y;

        int ppm_b = 0; /// Variable where one byte from file is saved
        int data_size = size_x * size_y * 3; /// Number of pixels to read * 3 colors

        /// Load pixel values byte to byte, check boundaries of file and EOF
        for(int i = 0; i < data_size && (ppm_b = fgetc(file)) != EOF; i++) {

            ppm_picture->data[i] = ppm_b;
        }
    }


    /**
     * If file was whole succesfuly read, then close file file and return 
     * pointer to structure
     */
    if(file != NULL) 
        fclose(file);

    return ppm_picture;
}

int ppm_write(struct ppm *p, const char *filename) {

    FILE *file;

    /// Error code
    int error = 0;

    /// Try to open file 'filename' in write mode
    file = fopen(filename, "w");

    if(file == NULL) {
    
        warning_msg("Chyba pri zapisovani do suboru %s\n", filename);
        error = -1;
    }

    /**
     * Try write out whole header to file: 
     * P6
     * x_size y_size
     * 255
     */
    if(!error) {

        if(fprintf(file, "P6\n") < 0) {

            fclose(file);
            warning_msg("Chyba pri zapisovani do suboru %s\n", filename);
            error = -1;
        }
    }

    if(!error) {

        if(fprintf(file, "%u %u\n", p->xsize, p->ysize) < 0) {

            fclose(file);
            warning_msg("Chyba pri zapisovani do suboru %s\n", filename);
            error = -1;
        }
    }

    if(!error) {

        if(fprintf(file, "255\n") < 0) {

            fclose(file);
            warning_msg("Chyba pri zapisovani do suboru %s\n", filename);
            error = -1;
        }
    }

    /// Calculate number of byte that will be written
    unsigned int data_size = p->xsize * p->ysize * 3;

    /// Write while no error occurs
    for(unsigned int i = 0; error != -1 && i < data_size; i++) {
    
        if(fputc(p->data[i], file) == EOF) {
        
            fclose(file);
            warning_msg("Chyba pri zapisovani suboru %s\n", filename);
            error = -1;
        }
    }

    /// Close file if whole file was written succesfuly
    if(file != NULL && error != -1) {
    
        fclose(file);
    }

    /// Return error number, 0 if no error occured
    return error; 
}
