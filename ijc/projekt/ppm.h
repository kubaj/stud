/**
 * ppm.h
 * Řešení: IJC-DU1, příklad b), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#ifndef _PPM_H
#define _PPM_H

/// Struct to store image data
struct ppm {
    unsigned xsize; /// Width of image
    unsigned ysize; /// Height of image
    char data[];    /// RGB values of pixels, celkem 3*xsize*ysize
};

/**
 * Function reads image from file, saves it into structure ppm and return 
 * pointer to this structure
 * @param filename Name of file to read
 * @return ppm* Pointer to structure ppm. NULL if error occured
 */
struct ppm *ppm_read(const char *filename);

/**
 * Function write ppm image to file.
 * @param *p Pointer to structure ppm to save
 * @param *filename Name of file to write
 * @return 0 if writing was succesful, negative number if error occured
 */
int ppm_write(struct ppm *p, const char *filename);

#endif
