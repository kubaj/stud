/**
 * error.h
 * Řešení: IJC-DU1, příklad b), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#ifndef _ERROR_H
#define _ERROR_H

/**
 * Print formatted warning message to stderr
 * @param *fmt Formated message
 */
void warning_msg(const char *fmt, ...);

/**
 * Print formatted error message to stderr and exit program
 * @param *fmt Formated message
 */
void fatal_error(const char *fmt, ...);

#endif
