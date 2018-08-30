#include "Game.h"
#include <assert.h>

#define MAXITERS 1000

/**
 * PRINTING FUNCTIONS
 */
void printEnterCommandMsg() {
    printf("Enter your command:\n");
}

void printFixedCellMsg() {
    printf("Error: cell is fixed\n");
}

void printSetErroneousPuzzleSolMsg() {
    printf("Puzzle solution erroneous\n");
}

void printSolvedSuccessfullyMsg() {
    printf("Puzzle solved successfully\n");
}

void printBoardContainsErroneousValsMsg() {
    printf("Error: board contains erroneous values\n");
}

void printValidationPassedMsg() {
    printf("Validation passed: board is solvable\n");
}

void printValidationFailedMsg() {
    printf("Validation failed: board is unsolvable\n");
}

void printBoardNotEmptyMsg() {
    printf("Error: board is not empty\n");
}

void printGeneratorFailedMsg() {
    printf("Error: puzzle generator failed\n");
}

void printNoMovesToUndoMsg() {
    printf("Error: no moves to undo\n");
}

void printUndoMsg(gameMove *move) {
    if (move->newVal==0 && move->prevVal==0) {
        printf("Undo %i,%i: from _ to _\n", (move->y +1), (move->x +1));
    }
    else if (move->prevVal == 0) {
        printf("Undo %i,%i: from %i to _\n", (move->y +1), (move->x +1), move->newVal);
    }
    else if (move->newVal == 0) {
        printf("Undo %i,%i: from _ to %i\n", (move->y +1), (move->x +1), move->prevVal);
    }
    else {
        printf("Undo %i,%i: from %i to %i\n", (move->y +1), (move->x +1), move->newVal, move->prevVal);
    }
}

void printNoMovesToRedoMsg() {
    printf("Error: no moves to redo\n");
}

void printRedoMsg(gameMove *move) {
    if (move->newVal==0 && move->prevVal==0) {
        printf("Redo %i,%i: from _ to _\n", (move->y +1), (move->x +1));
    }
    else if (move->prevVal == 0) {
        printf("Redo %i,%i: from _ to %i\n", (move->y +1), (move->x +1), move->newVal);
    }
    else if (move->newVal == 0) {
        printf("Redo %i,%i: from %i to _\n", (move->y +1), (move->x +1), move->prevVal);
    }
    else {
        printf("Redo %i,%i: from %i to %i\n", (move->y +1), (move->x +1), move->prevVal, move->newVal);
    }
}

void printSaveValidationFailedMsg() {
    printf("Error: board validation failed\n");
}

void printCellNotEmptyMsg() {
    printf("Error: cell already contains a value\n");
}

void exitErrorGame(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

void printSavedSuccessfullyMsg(char *path) {
    printf("Saved to: %s\n", path);
}

void printHintBoardUnsolvableMsg() {
    printf("Error: board is unsolvable\n");
}

void printHintMsg(int hintVal) {
    printf("Hint: set cell to %i\n", hintVal);
}

void printNumSolsMsg(long numSols) {
    printf("Number of solutions: %lu\n", numSols);
}

void printGoodBoardMsg() {
    printf("This is a good board!\n");
}

void printMultipleSolsExistMsg() {
    printf("The puzzle has more than 1 solutions, try to edit it further\n");
}

void printAutoFillMove(gameMove *move) {
    printf("Cell <%i,%i> set to %i\n", (move->y +1), (move->x +1), move->newVal);
}

void printBoardResetMsg() {
    printf("Board reset\n");
}

void printExitingMsg() {
    printf("Exiting...\n");
}

/**
 * Allocates memory for a (m*n)*(m*n) 2d array of cells. Exits if memory allocation fails, otherwise returns
   pointer to array
 * @param m rows in board box
 * @param n cols in board box
 * @return pointer to the gameBoard
 */
cell** createGameBoard(int m, int n) {
    int i, boardSize;
    cell **gameBoard;
    boardSize = (n*m);
    gameBoard = calloc(boardSize, sizeof(cell*));
    if (!gameBoard) {
        exitErrorGame("createGameBoard");
    }
    for (i=0 ; i<boardSize ; i++) {
        gameBoard[i] = calloc(boardSize, sizeof(cell));
        if (!gameBoard[i]) {
            exitErrorGame("createGameBoard");
        }
    }
    return gameBoard;
}

/**
 * Frees memory for gameBoard: (m*n)*(m*n) 2d array of cells
 * @param gameBoard matrix of cells
 * @param m rows in box
 * @param n cols in box
 * @param emptyCells pointer to the number of empty cells
 */
void freeGameBoard(cell **gameBoard, int m, int n, int *emptyCells) {
    int i, boardSize;
    boardSize = (m*n);
    for (i=0 ; i<boardSize ; i++) {
        free(gameBoard[i]);
    }
    free(gameBoard);
    *emptyCells = 0;
}

/**
 * set all isFixed fields to 0 in gameBoard
 * @param gameBoard pointer to cell matrix
 * @param N = m*n
 */
void unFixAllCells(cell **gameBoard, int N) {
    int i, j;
    for (i=0 ; i<N ; i++) {
        for (j=0 ; j<N ; j++) {
            gameBoard[i][j].isFixed = 0;
        }
    }
}

/**
 * initializes and allocates memory for an array with all possible values, sorted. Returns pointer to array
 * @param validNumArray pointer to N length array
 * @param N
 */
void resetPossibleNumbersArray(int *validNumArray, int N){
    int i;
    for (i=1; i<=N; i++) {
        validNumArray[i-1] = i;
    }
}

/**
 * changes the value of the x,y cell in the gameboard, prints error if cell is fixed
 * @param x column
 * @param y row
 * @param num to set in the cell
 * @param gameBoard pointer to the matrix of cells
 * @return
 */
int set(int x, int y, int num, cell **gameBoard) {
    if (gameBoard[x][y].isFixed) {
        printFixedCellMsg();
        return 0;
    } else {
        gameBoard[x][y].value = num;
        return 1;
    }
}

/**
 * checks if the num can be legaly placed in the row where the x,y cell is located
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param num for check
 * @param gameBoard pointer to the cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return boolean
 */
int isValidRowGame(int x, int y, int num, cell **gameBoard, int m, int n) {
    int i,N;
    N = m*n;
    /* make sure num does not appear in row x */
    for (i=0; i<N; i++) {
        if (i==y) {
            continue; /* this way an overide is evaluated to be valid */
        }
        if (num != 0 && gameBoard[x][i].value==num) {
            return 0;
        }
    }
    return 1; /* only reached if num is not in the row */
}

/**
 * checks if the num can be legaly placed in the column where the x,y cell is located
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param num for check
 * @param gameBoard pointer to the cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return boolean
 */
int isValidColGame(int x, int y, int num, cell **gameBoard, int m, int n)  {
    int i,N;
    N = m*n;
    /* */
    for (i=0; i<N; i++) {
        if (i==x) {
            continue; /* this way an overide is evaluated to be valid */
        }
        if (num != 0 && gameBoard[i][y].value==num) {
            return 0;
        }
    }
    return 1;  /* only reached if num is not in the col */
}

/**
 * checks if the num can be legaly placed in the box where the x,y cell is located
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param num for check
 * @param gameBoard pointer to the cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return boolean
 */
int isValidBoxGame(int x, int y, int num, cell **gameBoard, int m, int n){
    int boxRowIndex,boxColIndex,i,j;
    boxRowIndex = m*(x/m);
    boxColIndex = n*(y/n);
    /* start at top left corner of box, and iterate through box values */
    for (i=boxRowIndex; i<(boxRowIndex+m); i++) {
        for (j=boxColIndex; j<(boxColIndex+n); j++) {
            assert(gameBoard[i][j].value<=(m*n));
            if (i==x && j==y) {
                continue; /* this way an overide is evaluated to be valid */
            }
            if (num != 0 && gameBoard[i][j].value==num) {
                return 0;
            }
        }
    }
    return 1; /* only reached if num is not in the box */

}

/**
 * makes sure there are no erroneous values in board
 * @param gameBoard pointer to the cell matrix
 * @param m rows in box
 * @param n cols in box
 * @return boolean
 */
int isLegalBoard(cell **gameBoard, int m, int n) {
    int i,j;
    int vr,vc,vb;
    for (i=0; i<m*n; i++) {
        for (j=0; j<m*n; j++) {
            if (gameBoard[i][j].value != 0) { /* Check for cells that are not empty that no other cell contains their value */
                vr = isValidRowGame(i, j, gameBoard[i][j].value, gameBoard ,m, n);
                vc = isValidColGame(i, j, gameBoard[i][j].value, gameBoard ,m, n);
                vb = isValidBoxGame(i, j, gameBoard[i][j].value, gameBoard ,m, n);
                if (!(vr*vc*vb)) {
                    return 0;
                }
            }
        }
    }
    /* this point is reached if all is valid */
    return 1;
}

/**
 * checks if num is legel at column x and row y
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param num to check
 * @param gameBoard pointer to the cell matrix
 * @param m rows in box
 * @param n cols in box
 * @return boolean
 */
int isValidNumGame(int x, int y, int num, cell **gameBoard, int m, int n) {
    int vr,vc,vb;
    vr = isValidRowGame(x ,y, num, gameBoard, m, n);
    vc = isValidColGame(x ,y, num, gameBoard, m, n);
    vb = isValidBoxGame(x, y, num, gameBoard, m, n);
    return vr*vc*vb;
}

/**
 * if more than a single possible value exists, zero is returned, else the value is given
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param m rows in box
 * @param n columns in box
 * @param gameBoard pointer to cell matrix
 * @return int
 */
int getSinglePossibleValue(int x, int y, int m, int n, cell **gameBoard) {
    int i,cnt,value,N;
    N = m*n;
    cnt = 0;
    value = 0;
    /* checks all possible values, saves count and last valid value */
    for (i=1; i<=N; i++) {
        if (isValidNumGame(x, y, i, gameBoard, m, n)) {
            value = i;
            cnt++;
        }
    }
    if (cnt>1) {
        return 0;
    } else {
        return value;
    }
}

/**
 * automatically fills all cells that have a single possible value. assumes board is valid.
 * @param gameBoard pointer to cell matrix
 * @param urList doubly linked list used for undo redo operations
 * @param m rows in the gameBoard box
 * @param n cols in the gameBoard box
 * @param emptyCells pointer to the number of empty cells in the board
 */
void autoFill(cell **gameBoard, undoRedoList *urList, int m, int n, int *emptyCells) {
    /* declerations */
    int i,j,N,val,preVal;
    moveList *autoMoves;
    gameMove *move;
    N = m*n;

    /* create move list */
    autoMoves = createMoveList();
    /* create auxBoard */
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            /* get possible value */
            preVal = gameBoard[i][j].value;
            val = getSinglePossibleValue(i, j, m, n, gameBoard);
            if (val && preVal==0) { /* a single value exists */
                gameBoard[i][j].value = val; /* set new value to board */
                move = createNewGameMove(i, j, preVal, val); /* prepare move for undo move list */
                insertGameMove(autoMoves, move); /* add move to movelist */
                *emptyCells -= 1; /* update number of empty cells in the board */
                printAutoFillMove(move); /* print appropriate message */
            }
        }
    }
    /* if cells were filled, add to undo redo list */
    if (autoMoves->size>0) {
        insertUndoRedoNodeWithList(urList, autoMoves);
    }
    else {
        assert(autoMoves->size == 0);
        free(autoMoves);
    }
}

/**
 * set the isFixed value of each non empty cell to 1
 * @param gameBoard pointer to cell matrix
 * @param auxBoard auxillary int matrix
 * @param m rows in the gameBoard box
 * @param n cols in the gameBoard box
 */
void fixFullCells(cell **gameBoard, int **auxBoard, int m, int n) {
    int i,j,N;
    N = m*n;
    /* reset auxBoard, keep track of previously fixed cells */
    resetAuxBoard(auxBoard, m, n);
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            gameBoard[i][j].isFixed = auxBoard[i][j];
        }
    }
}

/**
 * updates the isError fields of each cell in the gameBoard
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 */
void updateErrorVals(cell **gameBoard, int m, int n) {
    int i,j,N;
    int check;
    N = m*n;
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            check = isValidNumGame(i, j, gameBoard[i][j].value, gameBoard, m, n);
            if (!check) {
                gameBoard[i][j].isError = 1;
            }
            else {
                gameBoard[i][j].isError = 0;
            }
        }
    }
}

/**
 * returns the number of empty cells in the game board
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return
 */
int countEmpty(cell **gameBoard, int m, int n) {
    int i,j,cnt;
    cnt = 0;
    for (i=0; i<m*n; i++) {
        for(j=0; j<m*n; j++) {
            if (gameBoard[i][j].value==0) {cnt++;}
        }
    }
    return cnt;
}

/**
 * undoes all moves in the current pointer of the undoRedoList, from head to tail
 * if command is not reset, prints undo message for each move that is undone
 * does not decrement current pointer
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param commandID id of current command
 * @param emptyCells pointer to number of empty cells in gameBoard
 * @param markErrors flag for printing
 */
void singleUndo(int m, int n, cell **gameBoard, undoRedoList *list, int commandID, int *emptyCells, int markErrors, int mode) {
    gameMove *currentMove;
    currentMove = list->current->moves->head;
    while (currentMove != NULL) {
        if ((currentMove->prevVal == 0) && currentMove->newVal != 0) {
            (*emptyCells)++; /* Changing cell value from non-zero to zero */
        }
        else if ((currentMove->prevVal != 0) && (currentMove->newVal == 0)) {
            (*emptyCells)--; /* Changing cell value from zero to non-zero */
        }
        /* Undo move on board */
        gameBoard[currentMove->x][currentMove->y].value = currentMove->prevVal;
        currentMove = currentMove->next;
    }
    /* Print board */
    printCommand(gameBoard, m, n, markErrors, mode);
    /* Print undo messages if necessary */
    if (commandID != 13) { /* Command is not reset */
        currentMove = list->current->moves->head;
        while (currentMove != NULL) {
            printUndoMsg(currentMove);
            currentMove = currentMove->next;
        }
    }
}

/**
 * redoes all moves in current pointer of undoRedoList
 * prints redo message for each move
 * does not increment current pointer
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param emptyCells pointer to number of empty cells in gameBoard
 * @param markErrors flag for printing
 */
void singleRedo(int m, int n, cell **gameBoard, undoRedoList *list, int *emptyCells, int markErrors, int mode) {
    gameMove *currentMove;
    currentMove = list->current->moves->head;
    while (currentMove != NULL) {
        if ((currentMove->prevVal == 0) && currentMove->newVal != 0) {
            (*emptyCells)--;
        }
        else if ((currentMove->prevVal != 0) && (currentMove->newVal == 0)) {
            (*emptyCells)++;
        }
        /* Undo move on board */
        gameBoard[currentMove->x][currentMove->y].value = currentMove->newVal;
        currentMove = currentMove->next;
    }
    /* Print board */
    printCommand(gameBoard, m, n, markErrors, mode);
    /* Print redo message */
    currentMove = list->current->moves->head;
    while (currentMove != NULL) {
        printRedoMsg(currentMove);
        currentMove = currentMove->next;
    }

}

/**
 * carries out all actions required for the autofill command
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param urList undo redo list
 * @param emptyCells pointer to num of empty cells
 * @param mode game mode (init=0, edit=1, solve=2)
 * @param markErrors flag for printing
 * @return success integer
 */
int autoFillCommand(int m, int n, cell **gameBoard, undoRedoList *urList, int *emptyCells, int *mode, int markErrors) {
    int flag, solvable;
    /* validate board before autofill */
    flag = isLegalBoard(gameBoard, m, n);
    if (!flag) {
        printBoardContainsErroneousValsMsg();
        return 0;
    } else {
        autoFill(gameBoard, urList, m, n, emptyCells);
        if (*emptyCells==0) {
            solvable = hasSolution(gameBoard, m, n);
            if (solvable) {
                printSolvedSuccessfullyMsg();
                emptyUndoRedoList(urList);
                *mode = 0;
            } else {
                printSetErroneousPuzzleSolMsg();
            }
        }
        printCommand(gameBoard, m, n, markErrors, *mode);
        return 1;
    }
}

/**
 * carries out all actions required for the num_solutions command
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return success integer
 */
int numSolutionCommand(cell **gameBoard, int m, int n) {
    long cnt;
    int flag;
    int **auxBoard;
    /* check if board is valid */
    flag = isLegalBoard(gameBoard, m, n);
    if (!flag) {
        printBoardContainsErroneousValsMsg();
        return -1;
    } else {
        /* setup the aux board */
        auxBoard = createAuxBoard(m, n);
        copyGameToAux(gameBoard, auxBoard, m, n);
        /* exaustive backtracking */
        cnt = exhaustiveBacktracking(auxBoard, m, n);
        printNumSolsMsg(cnt);
    }
    if (cnt==1) {
        printGoodBoardMsg();
    }
    else if (cnt > 1) {
        printMultipleSolsExistMsg();
    }
    freeAuxBoard(auxBoard, m, n);
    return 1;
}

/**
 * carries out all actions required for save command
 * @param path to save file to
 * @param gameBoard cell matrix
 * @param mode game mode (init=0, edit=1, solve=2)
 * @param m rows in board box
 * @param n cols in board box
 * @return success integer
 */
int saveCommand(char *path, cell **gameBoard, int mode, int m, int n) {
    int flag,error,solvable;
    int **auxBoard;
    if (mode==1) { /* edit mode */
        /* check if board contains errors */
        flag = isLegalBoard(gameBoard, m, n);
        if (flag) {
            /* check if board has solution */
            auxBoard = createAuxBoard(m, n);
            solvable = hasSolution(gameBoard, m, n);
            /* save if there is possible solution */
            if (solvable) {
                error = save(path, m, n, gameBoard, mode);
                if (error) {
                    printSavedSuccessfullyMsg(path);
                    fixFullCells(gameBoard, auxBoard, m, n);
                    freeAuxBoard(auxBoard, m, n);
                    return 1;
                } else {
                    freeAuxBoard(auxBoard, m, n);
                    return 0;
                }
            } else {
                /* board has no solution */
                printSaveValidationFailedMsg();
                freeAuxBoard(auxBoard, m, n);
                return 0;
            }
        } else {
            /* board contains errors, will not save in edit mode */
            printBoardContainsErroneousValsMsg();
            return 0;
        }
    } else { /* solve mode, no validation or checking is carried out */
        error = save(path, m, n, gameBoard, mode);
        if (error) {
            printSavedSuccessfullyMsg(path);
            return 1;
        } else {
            return 0;
        }
    }
}

/**
 * carries out set command procedure
 * @param x column in gameBoard
 * @param y row in gameBoard
 * @param num to set to cell
 * @param gameBoard pointer to cell matrix
 * @param emptyCells pointer to number of empty cells
 * @param urlist undo redo list
 * @param m rows in board box
 * @param n cols in board box
 * @param markErrors flag for printing
 * @param mode game mode (init=0,edit=1,solve=2)
 * @return success integer
 */
int setCommand(int x, int y, int num, cell **gameBoard, int *emptyCells, undoRedoList *urlist, int m, int n,  int markErrors, int *mode) {
    /* declerations */
    int prevNum, error,solvable;
    gameMove *move;

    prevNum = gameBoard[x][y].value;
    error = set(x ,y ,num, gameBoard);
    if (!error) {
        /* this means cells is fixed and cannot be changed */
        return 0;
    } else {
        /* handle empty cells */
        if (num!=0 && prevNum==0) {
            (*emptyCells)--;
        } else if (num==0 && prevNum!=0) {
            (*emptyCells)++;
        }
        /* handle undo redo list */
        deleteCurrentToLast(urlist); /* deletes moves following the new one */
        move = createNewGameMove(x, y, prevNum, num); /* create move for this set command */
        insertUndoRedoNodeWithMove(urlist, move); /* add to the undo redo list */
        printCommand(gameBoard, m, n, markErrors, *mode);
        /* validate and print if necessary */
        if (*emptyCells==0) {
            solvable = hasSolution(gameBoard, m, n);
            if (solvable) {
                printSolvedSuccessfullyMsg();
                emptyUndoRedoList(urlist);
                *mode = 0;
            } else {
                printSetErroneousPuzzleSolMsg();
            }
        }
        return 1;
    }
}

/**
 * carries out print command procedure
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @param markErrors printing flag
 */
void printCommand(cell **gameBoard, int m, int n, int markErrors, int mode) {
    /* update the errors before printing */
    updateErrorVals(gameBoard, m, n);
    if (mode == 1) { /* Mode is edit - markErrors is always 1 */
        printBoard(gameBoard, m, n, 1);
    }
    else {
        printBoard(gameBoard, m, n, markErrors);

    }
}

/**
 * carries out validation command
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return success integer
 */
int validateCommand(cell **gameBoard, int m, int n) {
    int isLegal,isSolvable;
    isLegal = isLegalBoard(gameBoard, m ,n);
    if (!isLegal) {
        printBoardContainsErroneousValsMsg();
        return 0;
    } else {
        isSolvable = hasSolution(gameBoard, m, n);
        if (isSolvable==1) {
            printValidationPassedMsg();
            return 1;
        } else if (isSolvable==0) {
            printValidationFailedMsg();
            return 0;
        } else { /* this means an error occured during the gurobi process */
            exitErrorGame("validateCommand");
            return -1;
        }
    }
}

/**
 * carries out get hint command
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @param x column
 * @param y row
 * @return success integer
 */
int hintCommand(cell **gameBoard, int m, int n, int x, int y) {
    int check,optStatus;
    int **auxBoard;
    /* check if board is legal */
    check = isLegalBoard(gameBoard, m, n);
    if (!check) {
        printBoardContainsErroneousValsMsg();
        return 0;
    } else if (gameBoard[x][y].isFixed) {
        printFixedCellMsg();
        return 0;
    } else if (gameBoard[x][y].value!=0) {
        printCellNotEmptyMsg();
        return 0;
    } else { /* solve board */
        auxBoard = createAuxBoard(m, n);
        optStatus = solveBoard(gameBoard, auxBoard, m, n, 0);
        if (optStatus==2) {
            printHintMsg(auxBoard[x][y]);
            freeAuxBoard(auxBoard, m, n);
            return 1;
        } else if (optStatus==3) {
            printHintBoardUnsolvableMsg();
            freeAuxBoard(auxBoard, m, n);
            return 0;
        } else {
            exitErrorGame("hintCommand");
            return 0;
        }
    }
}

/**
 * Tries to load from path to temporary board, if load was unsuccessful, returns NULL
 * if load was successful, frees memory of old gameBoard and returns address of new board
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param m rows in board box
 * @param n cols in board box
 * @param mode game mode (init=0, edit=1, solve=2)
 * @param emptyCells pointer to number of empty cells
 * @param com current command struct
 * @param markErrors flag for printing
 * @return success integer
 */
cell** solveCommand(cell **gameBoard, undoRedoList *list, int *m, int *n, int *mode, int *emptyCells, command *com, int *markErrors) {
    char *path = &(com->charArgs[0]);
    int tmp_m, tmp_n;
    cell **tmpBoard;
    /* Try to load from path */
    tmpBoard = load(path, &tmp_m, &tmp_n, emptyCells, com->CommandID);
    if (tmpBoard == NULL) { /* load was unsuccessful */
        return NULL;
    }
    else { /* load was successful */
        freeGameBoard(gameBoard, *m, *n, emptyCells);
        emptyUndoRedoList(list);
        *m = tmp_m;
        *n = tmp_n;
        *emptyCells = countEmpty(tmpBoard, *m, *n);
        /* set mode parameter to solve */
        *mode = 2;
        printCommand(tmpBoard, *m, *n, *markErrors, *mode);
        return tmpBoard;
    }
}

/**
 * * If user enters no path argument, frees old gameBoard memory, creates new 3x3 empty board and returns pointer to board
 * If user enters path argument, tries to load board. If load fails, returns NULL.
 * Otherwise frees old gameBoard memory, update mode and markErrors, and returns pointer to new board
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param m rows in board box
 * @param n cols in board box
 * @param mode game mode (init=0,edit=1,solve=2)
 * @param emptyCells
 * @param com
 * @param markErrors
 * @return
 */
cell** editCommand(cell **gameBoard, undoRedoList *list, int *m, int *n, int *mode, int *emptyCells, command *com, int *markErrors) {
    char *path;
    int N;
    int tmp_m, tmp_n;
    cell **tmpBoard;
    if (com->numParsedArgs == 0) { /* User does not enter path argument */
        /* Create 3x3 default size board */
        *m = 3;
        *n = 3;
        *mode = 1;
        freeGameBoard(gameBoard, *m, *n, emptyCells);
        emptyUndoRedoList(list);
        tmpBoard = createGameBoard(*m, *n);
        *emptyCells = 81;
        printCommand(tmpBoard, *m, *n, *markErrors, *mode);
        return tmpBoard;
    }
    else { /* User enters path argument */
        path = &(com->charArgs[0]);
        tmpBoard = load(path, &tmp_m, &tmp_n, emptyCells, com->CommandID);
        if (tmpBoard == NULL) { /* load was unsuccessful */
            return NULL;
        }
        else { /* load was successful */
            freeGameBoard(gameBoard, *m, *n, emptyCells);
            emptyUndoRedoList(list);
            /* Update m, n and N */
            *m = tmp_m;
            *n = tmp_n;
            N = ((*m)*(*n));
            *emptyCells = countEmpty(tmpBoard, *m, *n);
            unFixAllCells(tmpBoard, N);
            /* set mode parameter to edit */
            *mode = 1;
            /* set markErrors parameter to default value 1 */
            printCommand(tmpBoard, *m, *n, *markErrors, *mode);
            return tmpBoard;
        }
    }
}

/**
 * carries out markErrors command
 * @param markErrors current mark errors flag
 * @param com command struct, includes all parsed args and current command ID
 */
void markErrorsCommand(int *markErrors, command *com) {
    assert(com->numParsedArgs >0);
    *markErrors = com->intArgs[0];
    assert((*markErrors == 0) || (*markErrors == 1));
}

/**
 * undoes all moves in the current pointer of undoRedoList, then decrements current pointer
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param com command struct
 * @param emptyCells pointer to number of empty cells
 * @param markErrors print flag
 */
void undoCommand(int m, int n, cell **gameBoard, undoRedoList *list, command *com, int *emptyCells, int markErrors, int mode) {
    /* No moves to undo */
    if (list->current == NULL) {
        printNoMovesToUndoMsg();
    }
    else {
        /* If there are moves to undo (list->current != NULL)*/
        assert(list->size > 0);
        assert(list->head != NULL);
        assert(list->tail != NULL);
        singleUndo(m, n, gameBoard, list, com->CommandID, emptyCells, markErrors, mode);
        list->current = list->current->prev;
    }
}

/**
 * increments current pointer, then redoes all gameMoves in the current pointer
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param list undo redo list
 * @param emptyCells pointer to number of empty cells
 * @param markErrors print flag
 */
void redoCommand(int m, int n, cell **gameBoard, undoRedoList *list, int *emptyCells, int markErrors, int mode) {
    /* No moves to redo (list empty or current is at end of list) */
    if (list->current == list->tail) {
        printNoMovesToRedoMsg();
    }

    else { /* current != NULL OR current is before the head of the list (all moves have been undone) */
        if (list->current == NULL) { /* all moves have been undone */
            /* Increment current pointer */
            list->current = list->head;
        }
        else { /* current pointer is not at end of list (current->next exists) */
            /* increment current pointer */
            list->current = list->current->next;
        }
        assert(list->size > 0);
        assert(list->head != NULL);
        assert(list->tail != NULL);
        singleRedo(m, n, gameBoard, list, emptyCells, markErrors, mode);
    }
}

/**
 * undoes all moves to the beginning of undo redo list
 * clears undo-redo list
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBaord pointer to cell matrix
 * @param list undo redo list
 * @param commandID current command ID
 * @param emptyCells pointer to number of empty cells
 * @param markErrors print flag
 */
void resetCommand(int m, int n, cell **gameBaord, undoRedoList *list, int commandID, int *emptyCells, int markErrors, int mode) {
    while (list->current != NULL) {
        singleUndo(m, n, gameBaord, list, commandID, emptyCells, markErrors, mode);
        list->current = list->current->prev;
    }
    emptyUndoRedoList(list);
    printBoardResetMsg();
}

/**
 * checks that each cell's value is 0. If a cell contains non-zero value returns 0.
 * If a cell with zero value is fixed or is error, returns -1.
 * returns 1 if board is empty.
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return boolean
 */
int isEmptyBoard(cell **gameBoard, int m, int n) {
    int i, j, N, cellVal, cellFixed, cellError;
    N = (m * n);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cellVal = gameBoard[i][j].value;
            cellFixed = gameBoard[i][j].isFixed;
            cellError = gameBoard[i][j].isError;
            if (cellVal) { /* Cell is not empty */
                return 0;
            } else if (cellFixed || cellError) { /* Cell is 0 but fixed or error is 1 */
                return -1;
            }
        }
    }
    return 1; /* Board is empty */
}


/**
 * Goes over values in row x of gameBoard, and removes existing numbers different than 0 from validNumArray by setting validNumArray
  * in their respected index to LARGECONST
 * @param x row
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param validNumArray pointer to array to hold valid numbers for row
 */
void getValidFromRow(int x, int m, int n, cell **gameBoard, int *validNumArray) {
    int N, invalidNum, j;
    N = (m*n);
    for (j = 0; j < N; j++) {
        if (gameBoard[x][j].value != 0) {
            invalidNum = gameBoard[x][j].value;
            validNumArray[invalidNum - 1] = (2*N);
        }
    }
}

/**
 * Goes over values in row y of gameBoard, and removes existing numbers different than 0 from validNumArray by setting validNumArray
 * in their respected index to (2*N)
 * @param col
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param validNumArray pointer to array of valid number for column
 */
void getValidFromColumn(int col, int m, int n, cell **gameBoard, int *validNumArray) {
    int N, invalidNum , i;
    N = (m*n);
    for (i = 0; i < N; i++) {
        if (gameBoard[i][col].value != 0) {
            invalidNum = gameBoard[i][col].value;
            validNumArray[invalidNum - 1] = (2*N);
        }
    }
}

/**
 * Goes over the box containing gameBoard[x][y].value, and removes existing numbers different than 0 from validNumArray by setting
 * validNumArray in their respected index to (2*N)
 * @param row
 * @param col
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param validNumArray pointer to array of valid number for box
 */
void getValidFromBox(int row, int col, int m, int n, cell **gameBoard, int *validNumArray) {
    int N, i, j;
    int boxFirstRow, boxFirstColumn, invalidNum;
    N = (m*n);
    /* gameBoard[boxFirstRow][boxFirstColumn] is the first cell in the Sudoku box that contains gameboard[x][y] */
    boxFirstRow = m * (row / m);
    boxFirstColumn = n * (col / n);
    for (i = boxFirstRow; i < (boxFirstRow + m); i++) { /* Iterates over rows in box */
        for (j = boxFirstColumn; j < (boxFirstColumn + n); j++) { /* Iterates over columns in box */
            if (gameBoard[i][j].value != 0) {
                invalidNum = gameBoard[i][j].value;
                validNumArray[invalidNum - 1] = (2*N);
            }
        }
    }
}

/**
 * comperator used for qsort
 * @param a
 * @param b
 * @return integer (0 if equal)
 */
int intComparator(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

/**
 * Goes over the row x, the column y, and the Sudoku box that contains auxBoard[x][y] and "removes" existing numbers
 * different than 0 from validNumArray by setting numarray in their respected index to LARGECONST
 * @param row
 * @param col
 * @param m rows in board box
 * @param n cols in board box
 * @param gameBoard pointer to cell matrix
 * @param validNumArray pointer to array of valid numbers
 * @return count of valid numbers
 */
int getValidNumbers(int row, int col, int m, int n, cell **gameBoard, int *validNumArray){
    int N, counter,i;
    counter = 0;
    N = (m*n);
    getValidFromRow(row, m, n, gameBoard, validNumArray);
    getValidFromColumn(col, m, n, gameBoard, validNumArray);
    getValidFromBox(row, col, m, n, gameBoard, validNumArray);

    /* count number of valid numbers */
    for (i=0; i<N; i++) {
        if (validNumArray[i]<(2*N)) {
            counter++;
        }
    }
    qsort(validNumArray, N, sizeof(int), intComparator);
    /* Returns number of elements that are possible to use */
    return counter;
}

/**
 * updates all values and fields of the game board cells to 0
 * @param gameBoard pointer to cell matrix
 * @param N dimension of the board
 */
void nullifyGameBoard(cell **gameBoard, int N) {
    int i, j;
    for (i=0 ; i<N ; i++) {
        for (j=0 ; j<N ; j++) {
            gameBoard[i][j].value = 0;
            gameBoard[i][j].isFixed = 0;
            gameBoard[i][j].isError = 0;
        }
    }
}

/**
 * Given a sorted array of legal values, generates a random index and set's the legal number in index to cell
 * Cells that are filled become fixed
 * nullifies the valid Numbers array
 * @param gameBoard pointer to cell matrix
 * @param row
 * @param col
 * @param validCntr number of valid numbers
 * @param validNums pointer to array of validnumbers
 * @param N = m*n
 */
void setRandValToCell(cell **gameBoard, int row, int col, int validCntr, int *validNums, int N) {
    int randVal;
    randVal = validNums[(rand() % validCntr)];
    gameBoard[row][col].value = randVal;
    gameBoard[row][col].isFixed = 1;
    resetPossibleNumbersArray(validNums, N);
}

/**
 * sets all cells whose isFixed field is 0 to 0
 * @param gameBaord pointer to cell matrix
 * @param N = m*n
 */
void nullifyCellsNotFixed(cell **gameBaord, int N) {
    int i, j;
    for (i=0 ; i<N ; i++) {
        for (j=0 ; j<N ; j++) {
            if (!gameBaord[i][j].isFixed) {
                gameBaord[i][j].value = 0;
            }
        }
    }
}

/**
 * carries out all operations related to the generate command
 * @param gameBoard pointer to cell matrix
 * @param urList undo redo list
 * @param com current command struct
 * @param m rows in board box
 * @param n cols in board box
 * @param emptyCells pointer to number of empty cells
 * @return success integer
 */
int generateCommand(cell **gameBoard, undoRedoList *urList, command *com, int m, int n, int *emptyCells, int mode) {
    int iterCntr, i, startOver, randRow, randCol, N, X, Y, validCntr;
    int *validNums;
    int **auxBoard;
    gameMove *move;
    moveList *movesList;
    N = (m*n);
    startOver = 0;

    if (!isEmptyBoard(gameBoard, m, n)) { /* Board is not empty, terminate */
        assert(isEmptyBoard(gameBoard, m, n) != -1);
        printBoardNotEmptyMsg();
        return 0;
    }
    else { /* Board is empty */
        assert(*emptyCells == (N * N));
        X = com->intArgs[0];
        validNums = calloc(N, sizeof(int));
        if (!validNums) {
            exitErrorGame("generateCommand");
        }
        resetPossibleNumbersArray(validNums, N);

        /* Main Loop */
        for (iterCntr = 0; iterCntr < MAXITERS; iterCntr++) {
            i = 0;
            /* populates X random cells in gameBoard with random legal values */
            while (i < X) {
                randRow = rand() % N;
                randCol = rand() % N;
                if (gameBoard[randRow][randCol].value == 0) { /* Cell is empty */
                    validCntr = getValidNumbers(randRow, randCol, m, n, gameBoard, validNums);
                    if (validCntr > 0) { /* There is a legal value available for cell */
                        setRandValToCell(gameBoard, randRow, randCol, validCntr, validNums, N);
                        i++;
                    } else { /* No valid numbers for cell, need to start over */
                        startOver = 1; /* FLAG */
                        break;
                    }
                }
            }
            if (startOver || !(hasSolution(gameBoard, m, n))) { /* If startOver flag is on OR board has no solution */
                nullifyGameBoard(gameBoard, N);
                resetPossibleNumbersArray(validNums, N);
                continue;
            }
            else { /* Successfully generated X cells with random legal values and Board is Solvable */
                auxBoard = createAuxBoard(m, n);
                solveBoard(gameBoard, auxBoard, m, n, 0);
                copyAuxToGame(gameBoard, auxBoard, m, n);
                freeAuxBoard(auxBoard, m, n);
                movesList = createMoveList();
                Y = com->intArgs[1];

                unFixAllCells(gameBoard, N);
                i = 0;
                /* Generate Y random cells to fix */
                while (i<Y) {
                    randRow = rand() % N;
                    randCol = rand() % N;
                    if (!(gameBoard[randRow][randCol].isFixed)) {
                        move = createNewGameMove(randRow, randCol, 0, gameBoard[randRow][randCol].value);
                        insertGameMove(movesList, move);
                        gameBoard[randRow][randCol].isFixed = 1; /* Later changed back */
                        i++;
                    }
                }
                nullifyCellsNotFixed(gameBoard, N);
                unFixAllCells(gameBoard, N);
                /* Free memory and return success */
                free(validNums);
                emptyUndoRedoList(urList);
                insertUndoRedoNodeWithList(urList, movesList);
                *emptyCells = countEmpty(gameBoard, m, n);
                printCommand(gameBoard, m, n, 1, mode);
                return 1;
            }
        }
        /* If reached generator has been unsuccessful in generating a board under the MAXITERS constraint */
        free(validNums);
        nullifyGameBoard(gameBoard, N);
        printGeneratorFailedMsg();
        return 0;
    }
}


