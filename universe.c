#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "universe.h"
#include <stdlib.h>

// This is a structure that defines what the Universe is.
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// This function creates an instance of a Universe, and returns a pointer to it.
// The return type is (Universe *).
//
// It takes in two uint32s that state the dimensions of the universe, and a boolean
// value that indicates if the universe is toroidal.
//
// The code for creating the universe is similar to that from creating the Matrix from the Assignment 4 document.
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    //creating an instance of a Universe called universe
    Universe *universe = (Universe *) malloc(sizeof(Universe));
    if (rows > 0 && cols > 0) {
        universe->rows = rows;
        universe->cols = cols;
        universe->toroidal = toroidal;
        bool **ugrid = (bool **) calloc(rows, sizeof(bool *));
        for (uint32_t i = 0; i < rows; i += 1) {
            ugrid[i] = (bool *) calloc(cols, sizeof(bool));
        }
        universe->grid = ugrid;
    }
    return universe;
}

// This function de-allocates the memory on the Heap for the universe.
// It is a void function.
//
// It takes the pointer to the Universe to be de-allocated as its parameter.
void uv_delete(Universe *u) {
    //freeing Universe grid contents
    for (uint32_t i = 0; i < u->rows; i += 1) {
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    //freeing Universe grid
    free(u->grid);
    u->grid = NULL;
    //freeing Universe
    free(u);
    u = NULL;
}

// This function returns the number of rows that are in the universe.
// It returns a uint32 value.
//
// It takes the pointer to the Universe as its parameter.
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// This function returns the number of columns that are in the universe.
// It returns a uint32 value.
//
// It takes the pointer to the Universe as its parameter.
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// This function marks a particular cell as live.
// It is a void function.
//
// It takes in a pointer to a Universe, and two (uint32) row and column values.
// It only marks the cell if it is in-bounds of the Universe.
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    // checking to see if the cell is in-bounds, and marking it as true
    if ((uv_rows(u) > r) && (uv_cols(u) > c)) {
        u->grid[r][c] = true;
    }
}

// This function marks a particular cell as dead.
// It is a void function.
//
// It takes in a pointer to a Universe, and two (uint32) row and column values.
// It only marks the cell if it is in-bounds of the Universe.
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    // checking to see if the cell is in-bounds, and marking it as false
    if ((uv_rows(u) > r) && (uv_cols(u) > c)) {
        u->grid[r][c] = false;
    }
}

// This function retrieves the state of a particular cell.
// It returns a boolean value: either the value of a cell or
// false to indicate failure to retrieve a value of the cell.
//
// It takes in a pointer to a Universe, and two (uint32) row and column values.
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    // checking to see if cell is in-bounds, and returning value of cell.
    if ((uv_rows(u) > r) && (uv_cols(u) > c)) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

// This function populates the Universe with the values given from an input file.
// It returns a boolean value indicating whether or not the Universe was
// populated correctly.
// It reads the row-column pairs in the input file, which start on the second line.
//
// It takes in a pointer to a Universe, and a pointer to the file to write to.
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    //Scanning input file starting from second line
    while (fscanf(infile, "%" SCNu32 " %" SCNu32, &r, &c) != EOF) {
        if ((uv_rows(u) > r) && (uv_cols(u) > c)) {
            uv_live_cell(u, r, c);
        } else {
            //trying to populate invalid rows or column numbers
            return false;
        }
    }
    return true;
}

// This function counts all the live neighbors of each cell.
// The calculation is different based on whether it the Universe is toroidal or not.
// This function returns a uint32 count value.
//
// It takes in a pointer to a Universe, and two uint32s which represent the row and
// column of the Universe.
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;
    bool neighbor_bottom;
    bool neighbor_top;
    bool neighbor_left;
    bool neighbor_right;
    bool neighbor_top_left;
    bool neighbor_top_right;
    bool neighbor_bottom_left;
    bool neighbor_bottom_right;
    //Calculating the value of neighbor cells for toroidal universes or non-toroidal below (in else)
    if (u->toroidal) {
        neighbor_bottom = uv_get_cell(u, (r + 1) % uv_rows(u), c);
        neighbor_top = uv_get_cell(u, (r - 1 + uv_rows(u)) % uv_rows(u), c);
        neighbor_right = uv_get_cell(u, r, (c + 1) % uv_cols(u));
        neighbor_left = uv_get_cell(u, r, (c - 1 + uv_cols(u)) % uv_cols(u));
        neighbor_top_left
            = uv_get_cell(u, (r - 1 + uv_rows(u)) % uv_rows(u), (c - 1 + uv_cols(u)) % uv_cols(u));
        neighbor_top_right
            = uv_get_cell(u, (r - 1 + uv_rows(u)) % uv_rows(u), (c + 1) % uv_cols(u));
        neighbor_bottom_left
            = uv_get_cell(u, (r + 1) % uv_rows(u), (c - 1 + uv_cols(u)) % uv_cols(u));
        neighbor_bottom_right = uv_get_cell(u, (r + 1) % uv_rows(u), (c + 1) % uv_cols(u));
    } else {
        neighbor_bottom = uv_get_cell(u, r + 1, c);
        neighbor_top = uv_get_cell(u, r - 1, c);
        neighbor_right = uv_get_cell(u, r, c + 1);
        neighbor_left = uv_get_cell(u, r, c - 1);
        neighbor_top_left = uv_get_cell(u, r - 1, c - 1);
        neighbor_top_right = uv_get_cell(u, r - 1, c + 1);
        neighbor_bottom_left = uv_get_cell(u, r + 1, c - 1);
        neighbor_bottom_right = uv_get_cell(u, r + 1, c + 1);
    }
    //counting neighbors
    if (neighbor_bottom) {
        count += 1;
    }
    if (neighbor_top) {
        count += 1;
    }
    if (neighbor_left) {
        count += 1;
    }
    if (neighbor_right) {
        count += 1;
    }
    if (neighbor_top_left) {
        count += 1;
    }
    if (neighbor_top_right) {
        count += 1;
    }
    if (neighbor_bottom_left) {
        count += 1;
    }
    if (neighbor_bottom_right) {
        count += 1;
    }
    return count;
}

// This function prints the values of all the cells in the Universe array/grid.
// This function is void.
//
// It takes a pointer to the Universe and a pointer to the output file to output this
// printed array to.
// It prints to the given file an 'o' if the cell is alive and an '.' if the cell is dead.
void uv_print(Universe *u, FILE *outfile) {
    //traversing the rows and columns of each cell in the Universe grid.
    for (uint32_t i = 0; i < uv_rows(u); i += 1) {
        for (uint32_t j = 0; j < uv_cols(u); j += 1) {
            // printing to designated outfile
            if (uv_get_cell(u, i, j)) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
