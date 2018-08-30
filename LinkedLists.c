#include "LinkedLists.h"

/**
 * Used in case of memory allocation failure to print the function name in which the error occured and exit
 * @param functionName
 */
void exitErrorStructs(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

/**
 * Allocates memory for a struct representing a single game move, to be used for undo and redo
 * @param x - Board coordinates
 * @param y - Board coordinates
 * @param prevVal
 * @param newVal
 * @return gameMove*
 */
gameMove* createNewGameMove(int x, int y, int prevVal, int newVal) {
    gameMove *newGameMove = (gameMove*) malloc(sizeof(gameMove));
    if (!newGameMove) {
        exitErrorStructs("createNewGameMove");
    }
    else {
        newGameMove->x = x;
        newGameMove->y = y;
        newGameMove->prevVal = prevVal;
        newGameMove->newVal = newVal;
        newGameMove->prev = NULL;
        newGameMove->next = NULL;
    }
    return newGameMove;
}
/**
 * Allocates memory for a linked list struct that will contain undo redo nodes
 * @return undoRedoList*
 */
undoRedoList* createUndoRedoList() {
    undoRedoList *newList = (undoRedoList*) malloc(sizeof(undoRedoList));
    if (!newList) {
        exitErrorStructs("createUndoRedoList");
    }
    else {
        newList->head = NULL;
        newList->tail = NULL;
        newList->current = NULL;
        newList->size = 0;
    }
    return newList;
}
/**
 * Deletes last node of the undoRedoList, if such a node exists. Frees memory
 * @param urlist
 */
void deleteLastUndoRedo(undoRedoList *urlist) {
    undoRedoNode *temp = urlist->tail;
    if (temp != NULL) { /* List is not empty */
        assert(urlist->head != NULL);
        assert(urlist->size>0);
        if (urlist->current == urlist->tail) { /* current pointer needs decrementation */
            urlist->current = urlist->tail->prev;
        }
        /* decrement tail pointer */
        urlist->tail = urlist->tail->prev;
        /* free memory for tail node's move urlist */
        freeMoveList(temp->moves);
        urlist->size--;
        assert(urlist->size >= 0);
        if(urlist->size > 0) { /* List still is not empty (tail element exists) */
            assert(urlist->head != NULL);
            assert(urlist->tail != NULL);
            urlist->tail->next = NULL;
        }
        else {
            urlist->head = NULL;
        }
        free(temp); /* temp is freed if it is not NULL */
    }
}

/**
 * Allocates memory for a linked list struct that will contain all moves associated with a single undo or redo
 * @return moveList*
 */
moveList* createMoveList() {
    moveList *newMoveList = (moveList*) malloc(sizeof(moveList));
    if (!newMoveList) {
        exitErrorStructs("createMoveList");
    }
    else {
        newMoveList->head = NULL;
        newMoveList->tail = NULL;
        newMoveList->size = 0;
    }
    return newMoveList;
}

/**
 * Removes and frees all elements in move list from last to first. Frees memory for the struct moveList
 * @param list
 */
void freeMoveList(moveList *list) {
    while (list->size>0) {
        assert(list->head != NULL);
        assert(list->tail != NULL);
        deleteLastMove(list);
    }
    assert(list->head == NULL);
    assert(list->tail == NULL);
    free(list);
}
/**
 * Allocates memory for a new undo-redo element
 * @return undoRedoNode*
 */
undoRedoNode* createNewUndoRedoNode() {
    undoRedoNode *newNode = (undoRedoNode*) malloc(sizeof(undoRedoNode));
    if (!newNode) {
        exitErrorStructs("createNewUndoRedoNode");
    }
    else {
        newNode->moves = createMoveList();
        newNode->prev = NULL;
        newNode->next = NULL;
    }
    return newNode;
}
/**
 * Adds a new undo-redo element at the end of the undo-redo list, with single gameMove
 * Increments current pointer
 * @param list
 * @param move
 */
void insertUndoRedoNodeWithMove(undoRedoList *list, gameMove *move){
    undoRedoNode *newNode, *temp;
    newNode = createNewUndoRedoNode();
    insertGameMove(newNode->moves, move);
    /* List is empty */
    if (list->size == 0) {
        assert(list->head==NULL);
        assert(list->tail==NULL);
        list->head = newNode;
        list->tail = list->head;
        list->size++;
    } else { /* List is not empty */
        assert(list->head != NULL);
        assert(list->tail != NULL);
        temp = list->tail;
        newNode->prev = temp;
        list->tail->next = newNode;
        list->tail = newNode;
        list->size++;
    }
    /* Increment current pointer */
    list->current = list->tail;
}
/**
 * Adds a new undo-redo element at the end of the undo-redo list, with multiple gameMoves
 * Increments current pointer
 * Used by autofill and generate
 * @param list
 * @param mList
 */
void insertUndoRedoNodeWithList(undoRedoList *list, moveList *mList){
    undoRedoNode *newNode, *temp;
    newNode = createNewUndoRedoNode();
    free(newNode->moves);
    newNode->moves = mList;
    /* List is empty */
    if (list->size == 0) {
        assert(list->head==NULL);
        assert(list->tail==NULL);
        list->head = newNode;
        list->tail = list->head;
    } else { /* List is not empty */
        assert(list->head != NULL);
        assert(list->tail != NULL);
        temp = list->tail;
        newNode->prev = temp;
        temp->next = newNode;
        list->tail = newNode;
    }
    /* Increment current pointer and list size */
    list->current = list->tail;
    list->size++;
}
/**
 * Adds a new game move to a *gameMove* list
 * @param list
 * @param move
 */
void insertGameMove(moveList *list, gameMove *move) {
    gameMove *temp;
    if (list->head == NULL) {
        assert(list->size == 0);
        assert(list->tail == NULL);
        list->head = move;
        list->tail = list->head;
        list->size++;
        assert(list->size == 1);
    }
    else {
        assert(list->head != NULL);
        temp = list->tail;
        move->prev = temp;
        list->tail->next = move;
        list->tail = move;
        list->size++;
    }
}
/**
 * Removes the last game move element from a game Move list. Frees memory.
 * @param list
 */
void deleteLastMove(moveList *list) {
    gameMove *temp;
    temp = list->tail;
    if (temp != NULL) { /* List is not empty */
        list->tail = list->tail->prev;
        list->size--;
        assert(list->size >= 0);
        if (list->size > 0) {
            assert(list->head != NULL);
            assert(list->tail != NULL);
            list->tail->next = NULL;
        }
        else {
            list->head = NULL;
        }
        free(temp); /* temp is freed if it is not NULL */
    }
}

/**
 * Removes and frees all undo-redo elements in undo-redo list from last to first.
 * DOES NOT FREE THE LIST!
 * @param list
 */
void emptyUndoRedoList(undoRedoList *list) {
    while (list->size > 0) {
        assert(list->head != NULL);
        assert(list->tail != NULL);
        deleteLastUndoRedo(list);
    }
    /* nullify current pointer */
    list->current = NULL;
    assert(list->head == NULL);
    assert(list->tail == NULL);
}

/**
 * Given an undoRedo list deletes all the nodes from the next node to the current (if such a node exists) pointer until the end of the list
 * @param list
 */
void deleteCurrentToLast(undoRedoList *list) {
    undoRedoNode *currentTmp;
    if (list->size >0) {
        assert(list->head != NULL);
        assert(list->tail != NULL);
        currentTmp = list->current;
        while (list->tail != currentTmp) {
            deleteLastUndoRedo(list);
        }
    }
}

