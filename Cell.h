#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H
/**
 * Header file including the struct cell, of which the gameboard will be built
 */

struct cell {
    int value;
    int isFixed;
    int isError;
}; typedef struct cell cell;

#endif /*SUDOKU_CELL_H*/
