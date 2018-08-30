#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "Parser.h"
#include "ReadWrite.h"
#include "Cell.h"
#include "LinkedLists.h"
#include "Solver.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Game module includes all necessary functions in order to carry out the sudoku game.
 * createGameBoard - creates the matrix of cells that hold all information of the game state
 * freeGameBoard - frees all memory allocations associated with the game board
 * saveCommand - saves the current game to the file specified by path.
 * setCommand - changes the value of cell at given column and row to num
 * printCommand - prints the current game board according to spec
 * validateCommand - checks if the current board has a solution
 * solveCommand - carries out solve command, loads file and enters solve mode
 * editCommand - loads file or inits an empty board, changes the current mode to edit
 * markErrorsCommand - changes the mark errors flag
 * undoCommand - undos the last action and handles associated changes to linked list
 * redoCommand - redos the last action and handles associated changes to linked list
 * resetCommand - resets all actions carried out so far
 * numSolutionsCommand - counts all possible solutions to the board
 * generateCommand - generates a random board to solve with a given numbero of hints
 * hintCommand - provides the player with a hint for a given cell
 * autofillCommand - fills all cells where there is a single legal value.
 * isLegalBoard - checks if any values in the board are errors
 * */

cell** createGameBoard(int m, int n);
void freeGameBoard(cell **gameBoard, int m, int n, int *emptyCells);

int saveCommand(char *path, cell **gameBoard, int mode, int m, int n);
int setCommand(int x, int y, int num, cell **gameBoard, int *emptyCells, undoRedoList *urlist, int m, int n,  int markErrors, int *mode);
void printCommand(cell **gameBoard, int m, int n, int markErrors, int mode);
int validateCommand(cell **gameBoard, int m, int n);
cell** solveCommand(cell **gameBoard, undoRedoList *list, int *m, int *n, int *mode, int *emptyCells, command *com, int *markErrors);
cell** editCommand(cell **gameBoard, undoRedoList *list, int *m, int *n, int *mode, int *emptyCells, command *com, int *markErrors);
void markErrorsCommand(int *markErrors, command *com);
void undoCommand(int m, int n, cell **gameBoard, undoRedoList *list, command *com, int *emptyCells, int markErrors, int mode);
void redoCommand(int m, int n, cell **gameBoard, undoRedoList *list, int *emptyCells, int markErrors, int mode);
void resetCommand(int m, int n, cell **gameBaord, undoRedoList *list, int commandID, int *emptyCells, int markErrors, int mode);
int numSolutionCommand(cell **gameBoard, int m, int n);
int generateCommand(cell **gameBoard, undoRedoList *urList, command *com, int m, int n, int *emptyCells, int mode);
int hintCommand(cell **gameBoard, int m, int n, int x, int y);
int autoFillCommand(int m, int n, cell **gameBoard, undoRedoList *urList, int *emptyCells, int *mode, int markErrors);
int isLegalBoard(cell **gameBoard, int m, int n);

void printEnterCommandMsg();
void printExitingMsg();

#endif /*SUDOKU_GAME_H*/
