#ifndef SUDOKU_PARSER_H
#define SUDOKU_PARSER_H
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define DELIMITER " \t\r"

/**
 * Parser module includes the necessary structures and functions to parse a command from user input:
 * The command structure - binds all the necessary information regarding a valid command parsed from the user
 * the parseCommand function - checks for command validity, printing relevant invalid command/range messages if needed,
 * and parses the user input into the command structure
 */

struct command {
    int CommandID;
    int *intArgs; /* An array of the integer arguments parsed from the user */
    char *charArgs; /* An array of the character arguments parsed from the user */
    int numParsedArgs; /* Number of args parsed successfully */
}; typedef struct command command;

command* createCommand();
void resetCommandStruct(command *com);
void freeCommand(command *com);

int parseCommand(char *readBuffer, int mode, struct command *com, int m, int n, int *numArgs, int **legalCommands);

#endif /*SUDOKU_PARSER_H*/
