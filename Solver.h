#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "Stack.h"
#include "LinkedLists.h"
#include "gurobi_c.h"
#include "Aux.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/** The solver module includes functions used to solve a given sudoku board, or count the number of possible
*   solutions it has. The exaustive backtracking function uses a stack in place of recursion, and the solveBoard
*   function uses the Gurobi optimizer in order to solve the board.

*   hasSolution - retuns 1 if board is solvable, 0 if not, -1 on gurobi error
*   exhaustiveBacktracking - counts the number of solutions a board has
*   solveBoard - finds a solution to the gameBoard current state, and leaves it in the auxBoard. returns 2 if
*   optimal solution is found, 3 if problem is not feasable, and -1 on error.
*
*/

int hasSolution(cell **gameBoard, int m, int n);

long exhaustiveBacktracking(int **auxBoard, int m, int n);

int solveBoard(cell **gameBoard, int **auxBoard, int m, int n, int v);

#endif /*SUDOKU_SOLVER_H*/
