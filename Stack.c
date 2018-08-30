#include "Stack.h"


/**
 * exits the program, used in case of fault
 * @param functionName
 */
void exitErrorStack(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

/**
 * initializes a stack, with top NULL and count=0
 * @return pointer to a new stack
 */
stack* createStack() {
    /* declare vars */
    stack *pntr;
    /* allocate */
    pntr = calloc(1, sizeof(stack));
    if (!pntr) {
        exitErrorStack("createStack");
    }
    /* init members */
    pntr->count = 0;
    pntr->top = NULL;
    return pntr;
}

/**
 * add element to the top of the stack
 * @param callStack pointer to stack
 * @param state pointer to a state struct to be added
 */
void push(stack *callStack, callState *state) {
    /* insertion */
    state->next = callStack->top;
    callStack->top = state;
    /* increase count */
    callStack->count++;
}

/**
 * returns a pointer to the callState struct at the top of the stack
 * @param callStack pointer to sack
 * @return pointer to callState
 */
callState* top(stack *callStack) {
    return callStack->top;
}

/**
 * remove top element and retunr pointer
 * @param callStack pointer to the stack
 * @return pointer to poped callState
 */
callState* pop(stack *callStack) {
    /* declare and save return pointer */
    callState *temp;
    temp = callStack->top;
    /* update top and decrease counter */
    callStack->top = temp->next;
    callStack->count--;
    return temp;
}

/**
 * returns the size of the stack
 * @param callStack pointer to the stack
 * @return integer
 */
int size(stack *callStack) {
    return callStack->count;
}

/**
 * returns 1 if stack is empty size==0
 * @param callStack pointer to the stack
 * @return boolean
 */
int isEmpty(stack *callStack) {
    return callStack->count==0;
}

/**
 * delete and free all elements in the stack and the stack itself
 * @param callStack pointer to stack
 */
void freeStack(stack *callStack) {
    callState *temp;
    /* empty and free stack elements */
    while(callStack->top!=NULL) {
        temp = pop(callStack);
        freeCallState(temp);
    }
    /* free the stack */
    free(callStack);
}

/**
 * create a call state struct
 * @param x row
 * @param y column
 * @param stage of call
 * @param num to place in current cell
 * @return pointer to new callstate
 */
callState* createCallState(int x, int y, int stage, int num) {
    /* declerations and allocation */
    callState *pntr;
    pntr = calloc(1, sizeof(callState));
    if (!pntr) {
        exitErrorStack("createCallState");
    }
    /* populate members */
    pntr->x = x;
    pntr->y = y;
    pntr->stage = stage;
    pntr->num = num;
    pntr->next = NULL;
    return pntr;
}

/**
 * frees memory of a call state
 * @param state pointer to callState struct
 */
void freeCallState(callState *state) {
    free(state);
}

/* print the members of a call state */
void printCallState(callState *state) {
    printf("x: %i, y: %i, stage: %i, num: %i\n", state->x, state->y, state->stage, state->num);
}

/**
 * print all elements in the stack from top to bottom
 * @param callStack pointer to the stack
 */
void printStack(stack *callStack) {
    callState *temp = top(callStack);
    while (temp!=NULL) {
        printCallState(temp);
        temp = temp->next;
    }
    /* print newline */
    printf("\n");

}
