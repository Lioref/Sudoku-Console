#include "ReadWrite.h"



void printErrorPathInvalidMsg(int commandID) {
    if (commandID == 0) { /* Command is SOLVE */
        printf("Error: File doesn't exist or cannot be opened\n");

    }
    else { /* Command is EDIT */
        assert(commandID == 1);
        printf("Error: File cannot be opened\n");
    }
}

void exitErrorReadWrite(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

void printSaveFileErrorMsg() {
    printf("Error: File cannot be created or modified\n");
}

/**
 * fill all array entries with the null charectre (\0)
 * @param tempArr pointer to array
 * @param size array size
 */
void nullifyTempArr(char *tempArr, int size) {
    int i;
    for (i=0 ; i<size ; i++) {
        tempArr[i] = '\0';
    }
}

/**
 * returns the number of digits in the base 10 repersentation of num
 * @param num
 * @return repersentation length (int)
 */
int digitsBase10(int num) {
    if (num == 0) {
        return 1;
    }
    else {
        int digits = (int) floor((log10(num)) + 1);
        return digits;
    }
}

/**
 * returns a char array of the digits comprising num
 * @param num
 * @return pointer to char array
 */
char* intToCharArray(int num) {
    int i, digits;
    char *numArr;
    digits = digitsBase10(num);
    numArr = calloc(digits, sizeof(char));
    if (!numArr) {
        exitErrorReadWrite("intToCharArray");
    }
    for (i=0 ; i<digits ; i++) {
        numArr[digits-i-1] = (char) ((num % 10)+48);
        num /= 10;
    }
    return numArr;
}

/**
 * write the given number to a text file
 * @param fp pointer to the file
 * @param num number to write
 * @param isFixed flag
 * @param mode current game mode, (init=0,edit=1,solve=2)
 */
void writeNum(FILE *fp, int num, int isFixed, int mode) {
    int digits, temp;
    char *numArr, *nonFixedChar, *fixedChar;
    nonFixedChar = " ";
    fixedChar = ".";
    digits = digitsBase10(num);
    assert(digits>0);
    numArr = intToCharArray(num);
    temp = (int) fwrite(numArr, sizeof(char), digits, fp);
    if (temp != digits) {
        exitErrorReadWrite("writeNum"); /* Print error message with function name and exit */
    }
    if (isFixed || (mode==1 && num!=0)) { /* cell is fixed or we save from Edit mode */
        temp = (int) fwrite(fixedChar, sizeof(char), 1, fp);
        if (temp != 1) {
            exitErrorReadWrite("writeNum");
        }
    }
    /* Write whitespace */
    temp = (int) fwrite(nonFixedChar, sizeof(char), 1, fp);
    if (temp != 1) {
        exitErrorReadWrite("writeNum");
    }
    free(numArr);
}

/**
 * adds a new line to the open file
 * @param fp pointer to the file
 */
void writeNewLine(FILE *fp) {
    char *newLine;
    int temp;
    newLine = "\n";
    temp = (int) fwrite(newLine, sizeof(char), 1, fp);
    if (temp != 1) {
        exitErrorReadWrite("writeNewLine");
    }
}

/**
 * save the given gameBoard in the path specified, fixed values are saved according to
 * specification with a '.' following the number. the first row of the file include m and n
 * the dimensions of the board block
 * @param path to save game to
 * @param m pointer to the number of rows in the board box
 * @param n pointer to the number of cols in the board box
 * @param gameBoard pointer to cell matrix
 * @param mode current game mode (init=0,edit=1,solve=2)
 * @return success integer
 */
int save(char *path, int m, int n, cell **gameBoard, int mode) {
    FILE *fp;
    int i, j, boardSize;

    fp = fopen(path, "w");
    if (!fp) {
        printSaveFileErrorMsg();
        return 0;
    }
    /* Write m and n */
    writeNum(fp, m, 0, 0);
    writeNum(fp, n, 0, 0);
    writeNewLine(fp);
    boardSize = (m*n);
    /* write board cells to file */
    for (i=0 ; i<boardSize ; i++) {
        for (j=0 ; j<boardSize; j++) {
            writeNum(fp, gameBoard[i][j].value, gameBoard[i][j].isFixed, mode);
        }
        writeNewLine(fp);
    }
    fclose(fp);
    return 1;
}

/**
 * Allocates memory for an temporary Array, with entries all set to '\0'.
   Returns pointer to array.
 * @param size of array to allocate
 * @return pointer to char array of size given
 */
char* createTempArr(int size) {
    char *tempArr;
    tempArr = calloc(size, sizeof(int));
    if (!tempArr) { /* if tempArr is NULL pointer */
        exitErrorReadWrite("createTempArr");
    }
    nullifyTempArr(tempArr, size);
    return tempArr;
}

/**
 * reads m*n integers from file, and populates the cells in the gameBoard with the values read and fixed status.
 * @param fp pointer to file
 * @param gameBoard pointer to cell matrix
 * @param tempArr
 * @param size
 * @param row
 * @param m rows in board box
 * @param n cols in board box
 */
void readLine(FILE *fp, cell **gameBoard, char *tempArr, int size, int row, int m, int n) {
    int column, N, j, num;
    char c, *ptr;
    column = 0;
    j = 0;
    N = (m*n);
    /* Iterate on number of integers in row of gameBoard */
    while (column < N) {
        while(isdigit(c = (char) fgetc(fp))) { /* Read adjacent numbers into array */
            tempArr[j] = c;
            j++;
        }
        if (j>0) { /* Parse number in array to int and populate cell members */
            num = (int) strtol(tempArr, &ptr, 10);
            gameBoard[row][column].value = num;
            gameBoard[row][column].isFixed = (c == '.');
            assert(gameBoard[row][column].value >= 0);
            assert((gameBoard[row][column].isFixed == 0) || (gameBoard[row][column].isFixed == 1));
            column++;
            j = 0;
            nullifyTempArr(tempArr, size);
        }
    }
}

/**
 * counts number of empty cells in the gameBoard
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return number of empty cells
 */
int countEmptyCells(cell **gameBoard, int m, int n) {
    int i, j, N, emptyCntr;
    N = (m*n);
    emptyCntr = 0;
    for (i=0 ; i<N ; i++) {
        for (j=0 ; j<N ; j++) {
            if (gameBoard[i][j].value == 0) {
                emptyCntr++;
            }
        }
    }
    return emptyCntr;
}

/**
 * loads the sudoku board from the file at the given path into the gameBoard. number of
 * empty cells is updated, as well as the current board dimension variables m and n.
 * @param path to file
 * @param m pointer to number of rows in board box
 * @param n pointer to number of cols in board box
 * @param emptyCells pointer to the number of empty cells in current board
 * @param commandID current command id
 * @return matrix of cells populated with game from file
 */
cell** load(char *path, int *m, int *n, int *emptyCells, int commandID) {
    /* declerations */
    FILE *input;
    int blockRows,blockCols,numScanned,maxDigits,i;
    char tempChar;
    char *tempArr;
    cell **gameBoard;
    /* Checks if file exists, if not prints error and returns NULL */
    if ((access(path, F_OK ))) {
        printErrorPathInvalidMsg(commandID);
        return NULL;
    }
    /* open file  and validate*/
    input = fopen(path, "r");
    if (!input) {
        printf("load: exiting.");
        exitErrorReadWrite("load");
    }
    /* read the dimensions from file */
    numScanned = fscanf(input, "%i %i%c", &blockRows, &blockCols, &tempChar);
    if (numScanned!=3) {
        exitErrorReadWrite("load");
    }
    *m = blockRows; /* update dimensions of given row block pointer */
    *n = blockCols; /* update dimensions of given col block pointer */

    /* create game board */
    gameBoard = createGameBoard(blockRows, blockCols);
    if (!gameBoard) {
        exitErrorReadWrite("load");
    }
    /* create temporary array */
    maxDigits = digitsBase10(blockRows*blockCols);
    tempArr = createTempArr(maxDigits);
    if (!tempArr) {
        exitErrorReadWrite("load");
    }
    /* read from file line by line */
    for (i=0; i<(blockRows*blockCols); i++) {
        readLine(input, gameBoard, tempArr, maxDigits, i, blockRows, blockCols);
    }
    *emptyCells = countEmptyCells(gameBoard, *m, *n);
    /* free memory and close files */
    free(tempArr);
    fclose(input);

    /* return game board */
    return gameBoard;
}
