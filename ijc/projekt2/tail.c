/**                                                                                                                
 * tail.c                                                                        
 * Řešení: IJC-DU2, příklad a), 05.04.2016                                                                         
 * Autor: Jakub Kulich, FIT                                                                                        
 * Přeloženo: gcc (GCC) 5.3.0                                                                                      
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mgetline(char *line, int size, FILE *istream);

int main(int argc, char *argv[]) {

    /// Number of lines to write, default value is 10
    unsigned int noLines = 10;

    /// Number of argument where is filename
    int fileName = 0;

    /// Input stream
    FILE *inputStr = NULL;

    /**
     * Parse all arguments to variables 
     *
     * If argument is -n, next argument will be number of lines, if not, 
     * print error
     * If argument is not -n, it can be filename. If filename was in previous
     * argument, print error
     */

    int argc2 = argc;

    for(int i = 1; argc2 > 1; i++) {
    
        if(strcmp(argv[i],"-n") == 0) {
        
            if(i + 1 < argc) {

                if(sscanf(argv[++i], "%u", &noLines) != 1) { 

                    fprintf(stderr, "Error: Argument \"-n\" expects number\n");
                    return 1;
                }

            } else {
            
                fprintf(stderr, "Error: Argument \"-n\" expects value\n");
                return 1;
            }

            argc2-=2;
            
        } else if(fileName == 0) {
        
            fileName = i;
            argc2--;

        } else {
            
            fprintf(stderr, "Error: More arguments than expected\n");
            return 1;
        }
    }

    /**
     * If filename wasn't specified, set input stream to stdin.
     * In other case try open file. If file wasn't open succesfuly, print error
     */
    if(fileName == 0 || strcmp(argv[fileName],"-") == 0) { 
    
        inputStr = stdin;

    } else {
    
        inputStr = fopen(argv[fileName], "r");
        if(inputStr == NULL) {
        
            fprintf(stderr, "Error: Couldn't open file\n");
            return 1;
        }
    }

    /// Create array for n lines and buffer for one loaded line
    char lines[noLines][511];
    char line[511];
    
    /// Index of last loaded line in array
    unsigned int iLine = 0;

    while(mgetline(line, sizeof(line), inputStr) != EOF) {
    
        strcpy(lines[iLine%noLines],line);
        iLine++;
    }

    /**
     * Print all lines in queue, starting from last loaded index + 1. If less 
     * than n lines were loaded start from 0
     */
    for(unsigned int i = 0; i < noLines && i < iLine; i++) { 

        if(iLine < noLines) {
        
            printf("%s", lines[i]);

        } else {
        
            printf("%s", lines[(iLine+i)%noLines]);
        }
    }

    /// Close file if was opened
    if(fileName != 0 && inputStr != NULL) {
    
        fclose(inputStr);
    }

    return 0;
}

int mgetline(char *line, int size, FILE *istream) {

    /// Number of characters already loaded
    int loaded = 0;

    /// Buffer for loading characters
    int ch;

    /// Ensure warning message is printed only once
    int errorPrinted = 0;

    /// Load only max-1 characters, last byte reserved for '\0'
    size -= 1;

    while(1) {
    
        /// Load one character from input stream f
        ch = fgetc(istream);

        if(loaded < size) {
        
            if(ch == EOF) {
            
                line[loaded] = '\n';
                line[loaded + 1] = 0;

                return EOF;
            }

            line[loaded] = ch;
            loaded++;

            if(ch == '\n') {
            
                break;
            }


        } else {
        
            if(!errorPrinted) {
            
                fprintf(stderr,"Warning: input longer than buffer\n");
                errorPrinted = 1;
            }

            if(ch == '\n') {
            
                break;
            }
        }
    }

    /// Set last two byte to '\n' and '\0'
    line[loaded-1] = '\n';
    line[loaded] = 0;

    /// Return number of loaded characters
    return loaded;

}

