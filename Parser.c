#include "Parser.h"

/**
 * Used in case of memory allocation failure to print the function name in which the error occured and exit
 * @param functionName
 */
void exitErrorParser(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

/* printer utility for validate range */
void printErrorInvalidMarkErrorsRange() {
    printf("Error: the value should be 0 or 1\n");
}

/* printer utility for validate range */
void printErrorInvalidSetRange(int N) {
    printf("Error: value not in range 0-%i\n", N);
}

/* printer utility for validate range */
void printErrorInvalidGenerateRange(int N) {
    printf("Error: value not in range 0-%i\n", (N*N));
}

/* printer utility for validate range */
void printErrorInvalidHintRange(int N) {
    printf("Error: value not in range 0-%i\n", N);
}

/* printer utility for parseCommand */
void printErrorInvalidCommandMsg() {
    printf("ERROR: invalid command\n");
}

/**
 * Utility used by parseCommand to empty remaining characters until newline is read
 */
void emptyBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
/**
 * Creates a command structure, including memory allocation.
 * Returns 0 if memory allocation failed, 1 on success.
 * @return int
 */
command* createCommand() {
    command *com;
    com = calloc(1, sizeof(command));
    if (!com) return 0;
    com->intArgs = calloc(3, sizeof(int));
    if (!com->intArgs) return 0;
    com->charArgs = calloc(256, sizeof(char));
    if (!com->charArgs) return 0;
    return com;
}
/**
 * Free members of command structure, then the structure itself.
 * @param com
 */
void freeCommand(command *com) {
    free(com->charArgs);
    free(com->intArgs);
    free(com);
}
/**
 * Sets command structure members to 0
 * @param com
 */
void resetCommandStruct(command *com) {
    int i;
    com->CommandID = 0;
    com->numParsedArgs = 0;
    for (i=0 ; i<3 ; i++) {
        com->intArgs[i] = 0;
    }
    for (i=0 ; i<256 ; i++) {
        com->charArgs[i] = (char) 0;
    }
}

/**
 * Checks command is valid with regard to current game mode
 * returns 0 if command is not legal for current game mode, 1 if command is legal
 * @param mode
 * @param commandID
 * @param legalCommands
 * @return int
 */
int isLegalCommand(int mode, int commandID, int **legalCommands) {
    return legalCommands[commandID][mode]; /* legalCommands matrix is boolean */
}

/**
 * returns the commandID corresponding to the first word of user input, -1 if command is not recognized as valid
 * @param token
 * @param mode
 * @param legalCommands
 * @return int
 */
int getCommandID(char *token, int mode, int **legalCommands) {
    int i;
    char temp;
    /* convert to lower case */
    i=0;
    while (token[i]) {
        temp = (char) tolower(token[i]);
        token[i] = temp;
        i++;
    }
    /* match the first word to all possible commands and return corresponding id */
    if (!((int) strcmp(token, "solve")) && isLegalCommand(mode, 0, legalCommands)) {
        return 0;
    } else if (!((int) strcmp(token, "edit")) && isLegalCommand(mode, 1, legalCommands)) {
        return 1;
    } else if (!((int) strcmp(token, "mark_errors")) && isLegalCommand(mode, 2, legalCommands)) {
        return 2;
    } else if (!((int) strcmp(token, "print_board")) && isLegalCommand(mode, 3, legalCommands)) {
        return 3;
    } else if (!((int) strcmp(token, "set")) && isLegalCommand(mode, 4, legalCommands)) {
        return 4;
    } else if (!((int) strcmp(token, "validate")) && isLegalCommand(mode, 5, legalCommands)) {
        return 5;
    } else if (!((int) strcmp(token, "generate")) && isLegalCommand(mode, 6, legalCommands)) {
        return 6;
    } else if (!((int) strcmp(token, "undo")) && isLegalCommand(mode, 7, legalCommands)) {
        return 7;
    } else if (!((int) strcmp(token, "redo")) && isLegalCommand(mode, 8, legalCommands)) {
        return 8;
    } else if (!((int) strcmp(token, "save")) && isLegalCommand(mode, 9, legalCommands)) {
        return 9;
    } else if (!((int) strcmp(token, "hint")) && isLegalCommand(mode, 10, legalCommands)) {
        return 10;
    } else if (!((int) strcmp(token, "num_solutions")) && isLegalCommand(mode, 11, legalCommands)) {
        return 11;
    } else if (!((int) strcmp(token, "autofill")) && isLegalCommand(mode, 12, legalCommands)) {
        return 12;
    } else if (!((int) strcmp(token, "reset")) && isLegalCommand(mode, 13, legalCommands)) {
        return 13;
    } else if (!((int) strcmp(token, "exit")) && isLegalCommand(mode, 14, legalCommands)) {
        return 14;
    } else { /* Input is not valid command */
        return -1;
    }
}

/* makes sure that the given args are numeric for relevant command ids */
int validateArgType(char *token) {
    int i, numeric;
    i = 0;
    while (token[i]) {
        numeric = isdigit(token[i]);
        i++; /* incriment index */
        if (!numeric) {
            return 0;
        }
    }
    return 1; /* if entire array is numeric or empty, return true */
}

/**
 * Checks that args are in required range for command, prints appropriate error messages
 * @param m
 * @param n
 * @param commandID
 * @param arg
 * @param argIndex
 * @return int
 */
int validateRange(int m, int n, int commandID, int arg, int argIndex) {
    int check;
    if (commandID==2) { /* mark errors */
        check = (arg==0 || arg==1);
        if (check) {
            return 1;
        } else {
            printErrorInvalidMarkErrorsRange();
            return 0;
        }
    } else if (commandID==4) { /* set */
        if (argIndex == 2) { /* value parameter, enables setting 0 */
            check = (arg>=0 && arg<=(m*n));
        }
        else {
            check = (arg>0 && arg<=(m*n));
        }
        if (check) {
            return 1;
        } else {
            printErrorInvalidSetRange(m*n);
            return 0;
        }
    } else if (commandID==10) { /* hint */
        check = (arg>0 && arg<=(m*n));
        if (check) {
            return 1;
        } else {
            printErrorInvalidHintRange(n*m);
            return 0;
        }
    } else if (commandID==6) { /* generate */
        check = (arg>=0 && arg<=((m*n)*(m*n)));
        if (check) {
            return 1;
        } else {
            printErrorInvalidGenerateRange(m*n);
            return 0;
        }
    } else {
        return -1; /* false for commands that don't require range validation */
    }
}

/**
 * Sets the values of the read buffer array to zero
 * @param readBuffer
 */
void clearReadBuffer(char *readBuffer) {
    int i;
    for (i=0; i<257; i++) {
        readBuffer[i] = (char) 0;
    }
}

/**
 * Reads user input into char array, then parses the command into the struct command
 * Returns 0 if an invalid command has been issued and 1 on success in populating all struct command members.
 * Finally clears the readBuffer and empties the input buffer until next new line
 * @param readBuffer
 * @param mode
 * @param com
 * @param m
 * @param n
 * @param numArgs
 * @param legalCommands
 * @return
 */
int parseCommand(char *readBuffer, int mode, command *com, int m, int n, int *numArgs, int **legalCommands) {
    int numOfArgs, i, valid, intarg, tmp;
    char *token, c;

    /* Receive user input into readBuffer */
    i=0;
    while (((c = getchar()) != '\n' && c != EOF)) {
        readBuffer[i] = c;
        i++;
        /* Checks if user input is too long */
        if (i > 256) {
            printErrorInvalidCommandMsg();
            clearReadBuffer(readBuffer);
            emptyBuffer();
            return 0;
        }
    }
    if (i==0) {
        return -1;
    }
    /* Populate commandID member */
    token = strtok(readBuffer, DELIMITER);
    com->CommandID = getCommandID(token, mode, legalCommands);
    if (com->CommandID == -1) {
        printErrorInvalidCommandMsg();
        clearReadBuffer(readBuffer);
        return 0;
    }

    /* Parse the max number of args to be expected for command */
    numOfArgs = numArgs[com->CommandID];
    /* Parse the arguments */
    for (i=0 ; i<numOfArgs ; i++) {
        token = strtok(NULL, DELIMITER);
        if ((token == NULL) && (com->CommandID != 1)) {
            printErrorInvalidCommandMsg();
            return 0;
        }
        else if ((token == NULL) && com->CommandID == 1) {
            break;
        }
        /* For commands with int args (set, markErrors, generate, hint): check type and range of user input */
        if (com->CommandID==2 || com->CommandID==4 || com->CommandID==6 || com->CommandID==10) {
            /* Validate argument */
            valid = validateArgType(token);
            if (!valid) {
                printErrorInvalidCommandMsg();
                clearReadBuffer(readBuffer);
                return 0;
            };
            intarg = atoi(token);
            valid = validateRange(n , m, com->CommandID, intarg, i);
            if (!valid) { /* validateRange prints it's own error */
                clearReadBuffer(readBuffer);
                return 0;
            };
        }

        /* For commands "Edit", "Solve" and "Save": Populate member charArgs */
        if (com->CommandID == 0 || com->CommandID == 1 || com->CommandID == 9) {
            strcpy(com->charArgs, token);
            com->numParsedArgs++;
        }
        /* For other commands: Populate member intArgs */
        else if ((com->CommandID==2 || com->CommandID==4 || com->CommandID==6 || com->CommandID==10)){
            com->intArgs[i] = atoi(token);
            com->numParsedArgs++;
        }
    }
    /* For commands set and hint, change to zero based values and change order of args */
    if ((com->CommandID == 4) || (com->CommandID == 10)) {
        tmp = com->intArgs[1];
        com->intArgs[1] = (com->intArgs[0]-1);
        com->intArgs[0] = (tmp-1);
    }
    /* Validate num of parsed args */
    assert(((com->CommandID != 1) && (numOfArgs==com->numParsedArgs)) ||  ((com->CommandID == 1) && (numOfArgs >= com->numParsedArgs)));

    clearReadBuffer(readBuffer);
    return 1;
}
