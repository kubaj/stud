Documentation of CHA: C Header Analysis in Python 3 for IPP 2016/2017
Name and surname: Jakub Kulich
Login: xkulic03

# Program options
Program options are parsed by Python's `getopt()` function. Function parses options and creates dictionary of these options.

# Input and output
Input and output are selected by the program options `--input` and `--output`. If no file is selected then `STDIN` and `STDOUT` are defaultly used as input stream and output stream.

# Parsing input file
Input file is parsed using simple lexical analyser implemented using regexps. Before using lexical analyser comments and bodies of functions are removed. Lexical analyser outputs list of tokens.

# Processing tokens from lexical analyser
Tokens are split to the sublists where semicolon is used as delimiter. Each sublist now contains one function.

Each function is then processed: 
 1. Function is split to two parts:
    * Part left to the brackets - return value with function name
    * Part inside brackets - function arguments
 2. First part is taken - last identifier in list is function name. Other tokens are parts of the return value.
 3. Processing second part:
    * If second part is empty or contains keyword `void` then function is considered has no parameters.
    * If second part contains `...` then function is considered it has varieble-length arguments.
    * Second part is split to multiple lists with comma delimiter. Each new list is one argument. Argument name is removed from each argument if any exists. 

# Structure of data
There are multiple classes:
 * IPPDir - contains list of IPPFile
 * IPPFile - represents file and contains list of functions (IPPFunc)
 * IPPFunc - represents function and contains list of parameters (IPPParam)
 * IPPParam - represents function's parameter

Each class contains also other needed fields such as names. Each functions has overriden method `__str__()` and `__repr__()` so printing xml is possible only using command `print(IPPDir)`.