# The Game of Life
In this assignment, we create a simulation of The Game of Life by John Horton Conway. The Game of Life essentially works by operating on cells in a 2D matrix (the "universe"), and testing to see if they survive in the next generation using three rules--that is, each cell has either one of two states: alive or dead. The three rules are:
 
 1. If an alive cell has two or three neighbors, it survives.
 2. If a dead cell has three neighbors, it lives.
 3. All other cells die.
 
The parameters for creating the 2-D grid (for the universe) are inputted through using command-line arguments (detailed below), and the output of the program --the results of the Game of life for the given 
number of generations-- can be directed to any output file the user chooses. The program reads the data of the number of rows/columns and initial cells which are alive for the array from a user-given input file.
Another thing to note is that one of the options the user can specify is to make the universe toroidal--the matrix can be thought of as a closed-loop. If the user doesn't specify otherwise, then the results
of each generation for the Game of Life should be printed using ncurses.
## How to build the program
In order to build the program, the binary files first must be removed. So, in order to remove them, enter `$ make clean`. Then to compile the program, enter `$ make life`. Entering `$ make all` or `$ make` can also build the program.

## How to run the program
In order to run the program, enter `$ ./life (command-line options)`

## Command-line options
The options the program accepts for input are the following:
- -t: specifies that the universe will be toroidal
- -s: specifies that the ncurses should be silenced
- -n: specifies the number of generations to run the Game of Life
- -i infile: specifies the input file to read the input data from (default is standard input)
- -o outfile: specifies the file to print the output to (default is standard output)

## Scan-build
Scan-build revealed no errors when I ran it.

## Error Checking
In order to check for bad input such as row-column values being less than 0, improper files, and generation values being less than 0, I created if-statements that printed if improper input was given. They are in the form of "Malformed input: (specific error message)."
