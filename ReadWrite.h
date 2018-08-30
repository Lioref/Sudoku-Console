#ifndef SUDOKU_READWRITE_H
#define SUDOKU_READWRITE_H
#include "LinkedLists.h"
#include "Aux.h"
#include "Cell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

/**
 * handls output and input from sudoku files matching the specified format.
 * load - creates a game board populated according to values in the file
 * save - saves the current gameboard values to file.
 */

cell** load(char *path, int *m, int *n, int *emptyCells, int commandID);

int save(char *path, int m, int n, cell **gameBoard, int mode);

#endif /*SUDOKU_READWRITE_H*/
