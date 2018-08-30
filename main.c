#include "Aux.h"
#include <time.h>

int main() {
    /* Declarations */
    cell **gameBoard, **tmpBoard;
    undoRedoList *list;
    command *com;
    int m, n, emptyCells, mode, markErrors; /* Game variables */
    int exitFlag, res;
    int seed;
    int **legalComs, *argVec;
    char *readBuffer;

    /* Initializations */
    readBuffer = calloc(257, sizeof(char));
    if (!readBuffer) {
        printf("\"Error: main has failed\n");
        exit(1);
    }
    legalComs = createLegalComMatrix();
    argVec = createArgVector();
    m = 3; /* Set default values */
    n = 3;
    emptyCells = ((m*n)*(m*n));
    mode = 0;
    exitFlag = 0;
    markErrors = 1;
    gameBoard = createGameBoard(m, n);
    list = createUndoRedoList();
    com = createCommand();

    /* Randomization */
    seed = (int) time(NULL);
    srand(seed);

    printf("Sudoku\n------\n");
    /*Main game loop*/
    while (!exitFlag) {
       resetCommandStruct(com);
       printEnterCommandMsg();
       res = parseCommand(readBuffer, mode, com, m, n, argVec, legalComs);
       if (res != 1) {
           continue;
       } /* Command was invalid */
       if (com->CommandID == 0) { /* Solve command */
           tmpBoard = solveCommand(gameBoard, list, &m, &n, &mode, &emptyCells, com, &markErrors);
           if (tmpBoard) { /* Load was successful */
               gameBoard = tmpBoard;
           }
       }
       else if (com->CommandID == 1) { /* Edit command */
           tmpBoard = editCommand(gameBoard, list, &m, &n, &mode, &emptyCells, com, &markErrors);
           if (tmpBoard) { /* Load was successful */
               gameBoard = tmpBoard;
           }
       }
       else if (com->CommandID == 2) { /* Mark Errors command */
           markErrorsCommand(&markErrors, com);
       }
       else if (com->CommandID == 3) { /* Print Board command */
           printCommand(gameBoard, m, n, markErrors, mode);
       }
       else if (com->CommandID == 4) { /* Set command */
           setCommand(com->intArgs[0], com->intArgs[1], com->intArgs[2], gameBoard, &emptyCells, list, m, n, markErrors, &mode);
       }
       else if (com->CommandID == 5) { /* Validate command */
           validateCommand(gameBoard, m, n);
       }
       else if (com->CommandID == 6) { /* Generate command */
           generateCommand(gameBoard, list, com, m, n, &emptyCells, mode);
       }
       else if (com->CommandID == 7) { /* Undo command */
           undoCommand(m, n, gameBoard, list, com, &emptyCells, markErrors, mode);
       }
       else if (com->CommandID == 8) { /* Redo command */
           redoCommand(m, n, gameBoard, list, &emptyCells, markErrors, mode);
       }
       else if (com->CommandID == 9) { /* Save command */
           saveCommand(com->charArgs, gameBoard, mode, m, n);
       }
       else if (com->CommandID == 10) { /* Hint command */
           hintCommand(gameBoard, m, n, com->intArgs[0], com->intArgs[1]);
       }
       else if (com->CommandID == 11) { /* numSolutions command */
           numSolutionCommand(gameBoard, m, n);
       }
       else if (com->CommandID == 12) { /* Auto-fill command */
           autoFillCommand(m, n, gameBoard, list, &emptyCells, &mode, markErrors);
       }
       else if (com->CommandID == 13) { /* Reset command */
           resetCommand(m, n, gameBoard, list, com->CommandID, &emptyCells, markErrors, mode);
       }
       else { /* Exit command */
           exitFlag = 1;
           printExitingMsg();
       }
    }
    freeAll(legalComs, argVec, list, gameBoard, readBuffer, m, n, &emptyCells, com);
    return 0;
}







