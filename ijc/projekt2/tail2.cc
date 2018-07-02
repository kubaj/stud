/**                                                                                                             
 * tail2.cc
 * Řešení: IJC-DU2, příklad a), 05.04.2016
 * Autor: Jakub Kulich, FIT 
 * Přeloženo: g++ (GCC) 5.3.0
 */

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <queue>
#include <string>

int main(int argc, char *argv[]) {

    /// Number of lines to write, default value is 10
    unsigned int noLines = 10;
    
    /// Number of argument where is filename
    int fileName = 0;

    /// Pointer to input stream, default is stdin 
    std::istream *inputStr = &std::cin;
    std::ifstream ifs;

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

                if(!(std::istringstream(argv[++i]) >> noLines)) { 

                    std::cerr << "Error: Argument \"-n\" expects number\n";
                    return 1;
                }

            } else {
            
                std::cerr << "Error: Argument \"-n\" expects value\n";
                return 1;
            }

            argc2-=2;
            
        } else if(fileName == 0) {
        
            fileName = i;
            argc2--;

        } else {
            
            std::cerr << "Error: More arguments than expected\n";
            return 1;
        }
    }

    /// To increase program speed 
    std::ios::sync_with_stdio(false);

    /// Try to open filestream, if not succesful, print error
    if(fileName != 0 && strcmp(argv[fileName],"-")) { 
    
        ifs.open(argv[fileName], std::ifstream::in);

        if(!ifs.is_open()) {
        
            std::cerr << "Error: Couldn't open file\n";
            return 1;
        }

        inputStr = &ifs;
    }

    /// Create queue for last n lines and buffer for one loaded line
    std::queue<std::string> lines;
    std::string line;

    /** 
     * To ensure that in queue are n lines, first n lines is linesPrinted
     * decremented. If linesPrinted is 0, then each line that is pushed into 
     * queue must be one lined popped from queue
     */
    unsigned linesPrinted = noLines;

    while(std::getline(*inputStr, line)) {

        if(linesPrinted == 0) { 
        
            lines.pop();
        } else {
        
            linesPrinted--;
        }

        lines.push(line);
    }

    /// Print all lines in queue
    while(lines.size() > 0) {
    
        std::cout << lines.front() << std::endl;
        lines.pop();
    }
    
    /// Close file if was opened
    if(fileName != 0 && ifs.is_open()) {
    
        ifs.close();
    }

    return 0;
}
