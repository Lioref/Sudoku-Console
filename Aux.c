#include "Aux.h"

/**
 * prints error message, including name of the function where the error occured.
 * @param functionName name of function to include in the error message.
 */
void exitErrorAux(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

/**
 * create vector with the max number of arguments expected for each command ID
 * @return pointer to the created vector (*int)
 */
int* createArgVector() {
    int *argVec;
    int numArgs[15] = {1, 1, 1, 0, 3, 0, 2, 0, 0, 1, 2, 0, 0, 0, 0};
    argVec = calloc(15, sizeof(int));
    if (!argVec) {
        exitErrorAux("createArgVector");
    }
    memcpy(argVec, numArgs, 15*sizeof(int));
    return argVec;
}

/**
 * Allocates memory for 15x3 matrix, each row is a game command, and each column is a game mode:
   col 0 = init, col 1 = edit, col 2 = solve
 * @return pointer to matrix (*int)
 */
int** createLegalComMatrix() {
    int **legalCommands;
    int i;
    /* Memory allocation */
    legalCommands = calloc(15, sizeof(int*));
    if (!legalCommands) {
        exitErrorAux("createLegalComMatrix");
    }
    for (i=0; i<15; i++) {
        legalCommands[i] = calloc(3, sizeof(int));
        if (!legalCommands[i]) {
            exitErrorAux("createLegalComMatrix");
        }
    }
    /* Solve */
    legalCommands[0][0] = 1;
    legalCommands[0][1] = 1;
    legalCommands[0][2] = 1;
    /* Edit */
    legalCommands[1][0] = 1;
    legalCommands[1][1] = 1;
    legalCommands[1][2] = 1;
    /* MarkErrors */
    legalCommands[2][0] = 0;
    legalCommands[2][1] = 0;
    legalCommands[2][2] = 1;
    /* PrintBoard */
    legalCommands[3][0] = 0;
    legalCommands[3][1] = 1;
    legalCommands[3][2] = 1;
    /* Set */
    legalCommands[4][0] = 0;
    legalCommands[4][1] = 1;
    legalCommands[4][2] = 1;
    /* Validate */
    legalCommands[5][0] = 0;
    legalCommands[5][1] = 1;
    legalCommands[5][2] = 1;
    /* Generate */
    legalCommands[6][0] = 0;
    legalCommands[6][1] = 1;
    legalCommands[6][2] = 0;
    /* Undo */
    legalCommands[7][0] = 0;
    legalCommands[7][1] = 1;
    legalCommands[7][2] = 1;
    /* Redo */
    legalCommands[8][0] = 0;
    legalCommands[8][1] = 1;
    legalCommands[8][2] = 1;
    /* Save */
    legalCommands[9][0] = 0;
    legalCommands[9][1] = 1;
    legalCommands[9][2] = 1;
    /* Hint */
    legalCommands[10][0] = 0;
    legalCommands[10][1] = 0;
    legalCommands[10][2] = 1;
    /* NumSolutions */
    legalCommands[11][0] = 0;
    legalCommands[11][1] = 1;
    legalCommands[11][2] = 1;
    /* Auto-fill */
    legalCommands[12][0] = 0;
    legalCommands[12][1] = 0;
    legalCommands[12][2] = 1;
    /* Reset */
    legalCommands[13][0] = 0;
    legalCommands[13][1] = 1;
    legalCommands[13][2] = 1;
    /* Exit */
    legalCommands[14][0] = 1;
    legalCommands[14][1] = 1;
    legalCommands[14][2] = 1;

    return legalCommands;
}

/**
 * frees afformentioned legal command matrix
 * @param legalComMatrix pointer to matrix to be freed.
 */
void freeLegalComMatrix(int **legalComMatrix) {
    int i;
    for (i=0; i<15; i++) {
        free(legalComMatrix[i]);
    }
    free(legalComMatrix);
}

/**
 * prints the seperator row for board printing function
 * @param m - rows of box
 * @param n - cols of box
 */
void printSeperatorRow(int m, int n) {
    int i,N;
    N = n*m;
    for (i=0; i<(4*N+m+1);i++) {
        printf("-");
    }
    printf("\n");
}

/**
 * prints a row of a given gameBoard
 * @param gameBoard matrix of cell structs
 * @param m rows in box
 * @param n cols in box
 * @param row index to print from game board
 * @param markErrors flag indicating weather errors should be shown
 */
void printRow(cell **gameBoard, int m, int n, int row, int markErrors) {
    int i,N;
    N = m*n;
    for (i=0; i<N; i++) {
        /* print pipe if necessary */
        if (i%n==0) {
            printf("|");
        }
        /* print cell, consider empty cells */
        if (gameBoard[row][i].value==0) {
            printf("    ");
        } else { /* special print for fixed values */
            if (gameBoard[row][i].isFixed) {
                printf(" %2d.", gameBoard[row][i].value);
            } else { /* if mark errors is on, print with astrisk */
                if (markErrors && gameBoard[row][i].isError) {
                    printf(" %2d*", gameBoard[row][i].value);
                } else {
                    printf(" %2d ", gameBoard[row][i].value);
                }
            }
        }
    }
    /* print last pipe and newline*/
    printf("|\n");
}

/**
 * prints the gameboard, calls printSeperator and printRow
 * @param gameBoard gameBoard matrix of cell structs
 * @param m rows in box
 * @param n cols in box
 * @param markErrors
 */
void printBoard(cell **gameBoard, int m, int n, int markErrors) {
    /* declare vars */
    int i,N;
    N = m*n;
    for (i=0;i<N;i++) {
        /* additional separator where needed */
        if (i%m==0) {
            printSeperatorRow(m, n);
        }
        /* print row values */
        printRow(gameBoard, m, n, i, markErrors);
    }
    /* last separator row */
    printSeperatorRow(m,n);
}

/**
 * prints a row from the auxillary board
 * @param auxBoard integer matrix
 * @param m rows in box
 * @param n col in box
 * @param row
 */
void printAuxRow(int **auxBoard, int m, int n, int row) {
    int i,N;
    N = m*n;
    for (i=0; i<N; i++) {
        /* print pipe if necessary */
        if (i%n==0) {
            printf("|");
        }
        /* print cell, consider empty cells */
        if (auxBoard[row][i]==0) {
            printf("    ");
        } else {
            printf(" %2d ", auxBoard[row][i]);
            }
        }
    /* print last pipe and newline*/
    printf("|\n");
    }

/**
 * prints the auxillary board, calls printRow and printSeperator
 * @param auxBoard integer matrix of dimensions m*n
 * @param m rows in box
 * @param n col in box
 */
void printAuxBoard(int **auxBoard, int m, int n) {
    /* declare vars */
    int i,N;
    N = m*n;
    for (i=0;i<N;i++) {
        /* additional separator where needed */
        if (i%m==0) {
            printSeperatorRow(m, n);
        }
        /* print row values */
        printAuxRow(auxBoard, m, n, i);
    }
    /* last separator row */
    printSeperatorRow(m,n);
}

/**
 * creates a auxillary board, with dimensions m*n
 * @param m rows in box
 * @param n cols in box
 * @return pointer to int matrix
 */
int** createAuxBoard(int m, int n) {
    /* declare vars */
    int **pntr;
    int i, N;
    /* allocate array of integer pointer*/
    N = m*n;
    pntr = calloc(N, sizeof(int*));
    if (!pntr) {
        exitErrorAux("createAuxBoard. Exiting.");
    }
    for (i=0; i<N; i++) {
        pntr[i] = calloc(N, sizeof(int));
        if (!pntr[i]) {
            exitErrorAux("createAuxBoard. Exiting.");
        }
    }
    return pntr;
}

/**
 * resets auxillary board values to zero
 * @param auxBoard integer matrix
 * @param m rows in box
 * @param n cols in box
 */
void resetAuxBoard(int **auxBoard, int m, int n) {
    int i,j;
    for (i=0; i<(m*n); i++) {
        for (j=0; j<(m*n); j++) {
            auxBoard[i][j] = 0;
        }
    }
}

/**
 * frees the auxillary board
 * @param auxBoard
 * @param m
 * @param n
 */
void freeAuxBoard(int **auxBoard, int m, int n) {
    int i;
    /* free inner arrays */
    for (i=0; i<(m*n); i++) {
        free(auxBoard[i]);
    }
    /* free outer array */
    free(auxBoard);
}

/**
 * copies all the values from the game board to the auxillary board
 * @param gameBoard matrix of cell structs
 * @param auxBoard int matrix
 * @param m rows in box
 * @param n cols in box
 */
void copyGameToAux(cell **gameBoard, int **auxBoard, int m, int n) {
    int i,j;
    for (i=0; i<(m*n); i++) {
        for (j=0; j<(m*n); j++) {
            auxBoard[i][j] = gameBoard[i][j].value;
        }
    }
}

/**
 * copies all the values from the auxillary board back to the game board
 * @param gameBoard matrix of cell structs
 * @param auxBoard integer matrix
 * @param m rows in box
 * @param n cols in box
 */
void copyAuxToGame(cell **gameBoard, int **auxBoard, int m, int n) {
    int i,j;
    for (i=0; i<m*n; i++) {
        for (j=0; j<m*n; j++) {
            gameBoard[i][j].value = auxBoard[i][j];
        }
    }
}

/**
 * frees all the structures used by the main function
 * @param legalComs matrix of legal commands per mode
 * @param argVec number of arguments each command accepts
 * @param list undo redo linked list
 * @param gameBoard matrix of cell structs
 * @param readBuffer array used to read commands from input
 * @param m rows in box
 * @param n cols in box
 * @param emptyCells number of empty cells left in current board
 * @param com command structure
 */
void freeAll(int **legalComs, int *argVec, undoRedoList *list, cell **gameBoard, char *readBuffer, int m, int n, int *emptyCells, command *com) {
    freeLegalComMatrix(legalComs);
    free(argVec);
    emptyUndoRedoList(list);
    free(list);
    freeGameBoard(gameBoard, m, n, emptyCells);
    freeCommand(com);
    free(readBuffer);
}
