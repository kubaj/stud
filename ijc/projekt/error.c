/**
 * error.c
 * Řešení: IJC-DU1, příklad b), 15.03.2016
 * Autor: Jakub Kulich, FIT
 * Přeloženo: gcc (GCC) 5.3.0
 */

#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...) {

    va_list arg;

    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);

}

void fatal_error(const char *fmt, ...) {

    va_list arg;

    int length;
    char *newFmt;
    const char newFmtString[] = "CHYBA: %s";

    length = snprintf(NULL, 0, newFmtString, fmt);
    newFmt = malloc(length + 1);
    snprintf(newFmt, length + 1, newFmtString, fmt);

    va_start(arg, fmt);
    vfprintf(stderr, newFmt, arg);
    va_end(arg);

    free(newFmt);

    exit(1);

}
