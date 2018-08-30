#ifndef SUDOKU_AUX_H
#define SUDOKU_AUX_H
#include "Game.h"
#include "Cell.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/** Auxillary module including utilities for the main function and its associated structures.
 *
 * createArgVector - creates a vector of number of expected arguments for each command.
 * createLegalComMatrix - creates a matrix of legal commands for each mode.
 * freeLegalComMatrix - frees legal command matrix.
 * printBoard - prints the game sudoku board.
 * createAuxBoard - creates and auxillary matrix with dimensions m*n.
 * freeAuxBoard - frees auxillary matrix
 * copyGameToAux - copies values from game board to the auxilary board
 * copyAuxToGame - copies values from the auxillary board the game board.
 * printAuxBoard - prints the auxillary board.
 * printAuxRow - utility for the printAuxBoard functions
 * freeAll - functions used at termination of the game, all resources are freed.
 * **/

int* createArgVector();
int** createLegalComMatrix();
void freeLegalComMatrix(int **legalComMatrix);
int** createAuxBoard(int m, int n);
void resetAuxBoard(int **auxBoard, int m, int n);
void freeAuxBoard(int **auxBoard, int m, int n);
void freeAll(int **legalComs, int *argVec, undoRedoList *list, cell **gameBoard, char *readBuffer, int m, int n, int *emptyCells, command *com);
void printBoard(cell **gameBoard, int m, int n, int markErrors);
void copyGameToAux(cell **gameBoard, int **auxBoard, int m, int n);
void copyAuxToGame(cell **gameBoard, int **auxBoard, int m, int n);


#endif /*SUDOKU_AUX_H*/
