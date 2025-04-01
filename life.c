#include <stdio.h>
#include <stdlib.h>
#include "universe.h"
#include <ncurses.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

#define DELAY 50000

// This is a function that swaps two Universe type pointers.
// It doesn't return anything.
// M and N are pointers to a pointers of Universes.
// M and N must already point to Universes.
//
// Code was inspired by swap function for integers in The C Programming Language by
// Brian Kernighan and Dennis Ritchie.
void swap_universe(Universe **M, Universe **N) {
    Universe *temporary_universe;
    temporary_universe = *M;
    *M = *N;
    *N = temporary_universe;
}

// This function implements Game of Life's three life rules.
// This is a void function.
//
// If the cell in A is a live cell has two or three live neighbors, call uv_live_cell() with B.
// If a cell in B is a dead cell in A has three live neighbors, call uv_live_vell() with B.
// If the two above conditions are not met, call uv_dead_Cell() with B.
//
// A and B must be pointers to already initialized universes, i is the row index
// and j is the column index of the universe.
void life_rules(Universe *A, Universe *B, uint32_t i, uint32_t j) {
    //three rules of life given below
    if (uv_get_cell(A, i, j) && (uv_census(A, i, j) == 2 || uv_census(A, i, j) == 3)) {
        uv_live_cell(B, i, j);
    } else if ((uv_get_cell(A, i, j) == false) && (uv_census(A, i, j) == 3)) {
        uv_live_cell(B, i, j);
    } else {
        uv_dead_cell(B, i, j);
    }
}

// This function parses command-line arguments, initializes two Universes, and reads data
// from an input file to play The Game of Life with or without ncurses (depending on
// user input.) The output of this function is also directed to a file.
// This function returns an integer.
//
// This function takes in the command-line variables as its input.
// It allows the user to enter in -t, -s, -n value, -i file, -o file as the options.
// If the user enters the -s option, then ncurses is silenced - that is,the if-statement
// with !silence below is ignored and the else is executed.
// Regardless of whether -s is entered, the state of the cells at the final generation is
// printed out at the end to the outfile the user specified.
int main(int argc, char **argv) {
    int32_t opt = 0;
    bool toroidal = false;
    bool silence = false;
    int32_t generations = 100;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    uint32_t rows, cols;
    Universe *A;
    Universe *B;

    //parsing command-line options
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': silence = true; break;
        case 'n':
            generations = (int32_t) strtoul(optarg, NULL, 10);
            // checking for negative generations
            if (generations < 0) {
                fprintf(stderr, "Malformed input: generations cannot be negative.\n");
                return (1);
            }
            break;
        case 'i':
            infile = fopen(optarg, "r");
            // checking to see if file exists
            if (infile == NULL) {
                fprintf(stderr, "Error opening %s.\n", optarg);
                return (1);
            }
            break;
        case 'o': outfile = fopen(optarg, "w"); break;
        default:
            // if incorrect options are given, return an error
            return (1);
        }
    }
    fscanf(infile, "%" SCNu32 "%" SCNu32, &rows, &cols);
    // checking for incorrect row-column values
    if ((rows < 1) || (cols < 1)) {
        fprintf(stderr, "Malformed input: Number of rows and columns both must be atleast 1.\n");
        return (1);
    }
    A = uv_create(rows, cols, toroidal);
    B = uv_create(rows, cols, toroidal);
    // if universe was properly populated, execute below statement
    if (uv_populate(A, infile)) {

        if (!silence) {
            // branch for not-silenced ncurses option
            // initialize ncurses
            initscr();
            curs_set(FALSE);
            // looping through each generation
            for (int32_t g = 0; g < generations; g += 1) {
                clear();
                for (uint32_t i = 0; i < uv_rows(A); i += 1) {
                    for (uint32_t j = 0; j < uv_cols(A); j += 1) {
                        // printing array element to ncurses
                        if (uv_get_cell(A, i, j)) {
                            mvprintw(i, j, "o");
                        } else {
                            mvprintw(i, j, " ");
                        }
                        life_rules(A, B, i, j);
                    }
                }
                swap_universe(&A, &B);
                refresh();
                usleep(DELAY);
            }
            // end ncurses
            endwin();
        } else {
            // branch for silenced ncurses option
            // looping through each generation
            for (int32_t g = 0; g < generations; g += 1) {
                for (uint32_t i = 0; i < uv_rows(A); i += 1) {
                    for (uint32_t j = 0; j < uv_cols(A); j += 1) {
                        life_rules(A, B, i, j);
                    }
                }
                swap_universe(&A, &B);
            }
        }
        // printing array
        uv_print(A, outfile);
        //closing files
        fclose(infile);
        fclose(outfile);
        //freeing Universe memory
        uv_delete(A);
        uv_delete(B);
    } else {
        // checking for malformed input
        fprintf(stderr, "Malformed input.\n");
    }
}
