#ifndef SUDOKU_STACK_H
#define SUDOKU_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * Stack data structure that holds call states for the exaustive backtracking function. Implemented
 * with a linked list, Includes standard stack methods:
 *  - Top
 *  - Pop
 *  - Push
 *  - Size
 *  - isEmpty
 *  - createStack
 *  - freeStack
 * as well as a constructor and destructor for the callstate struct
 */


/* struct to hold all variables relevant to recursion call and serve as stack node */
struct callState {
    int x;
    int y;
    int stage;
    int num;
    struct callState *next;
}; typedef struct callState callState;

/* stack for recursive function calls */
struct stack {
    int count; /* number of elements in the stack */
    callState *top; /* pointer to top of stack */
}; typedef struct stack stack;

/* stack prototypes */
/* initializes a stack, with top NULL and count=0 */
stack* createStack();

/* delets all elemts in the stack and the stack itself, frees all memory */
void freeStack(stack *callStack);

/* create a call state */
callState* createCallState(int x, int y, int stage, int num);

/* free memeory of callState */
void freeCallState(callState *state);

/* returns pointer to top element of stack */
callState* top(stack *callStack);

/* removes top element and returns a pointer to it */
callState* pop(stack *callStack);

/* add element to the top of the stack */
void push(stack *callStack, callState *state);

/* returns the size of the stack */
int size(stack *callStack);

/* returns 1 if stack is empty size==0 */
int isEmpty(stack *callStack);


#endif /*SUDOKU_STACK_H*/
