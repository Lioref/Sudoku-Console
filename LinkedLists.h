#ifndef SUDOKU_STRUCTURES_H
#define SUDOKU_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LinkedLists module includes:
 * The gameMove structure - represents a single user game move (a single set move)
 * The moveList structure - a doubly linked list of gameMoves
 * The undoRedoNode structure - that is used as the node of the undo-redo list
 * The UndoRedoList structure - doubly linked list of undoRedoNodes
 * The linked-list data structure functions needed to implement the undo-redo list as specified
 *
 * The undo-redo list is a doubly linked list of struct undoRedoNodes, such that the data of each node is a pointer to a list of
 * struct gameMoves associated with a single undo or redo command
 */

struct innerNode {
    int x;
    int y;
    int prevVal;
    int newVal;
    struct innerNode* next;
    struct innerNode* prev;
}; typedef struct innerNode gameMove;

struct innerList {
    gameMove *head;
    gameMove *tail;
    int size;
}; typedef struct innerList moveList;

struct outerNode {
    moveList* moves;
    struct outerNode* next;
    struct outerNode* prev;
}; typedef struct outerNode undoRedoNode;

struct outerList {
    undoRedoNode *head;
    undoRedoNode *tail;
    undoRedoNode *current; /* A pointer to the current undoRedoNode in the game */
    int size;
}; typedef struct outerList undoRedoList;

gameMove* createNewGameMove(int x, int y, int prevVal, int newVal);
undoRedoList* createUndoRedoList();
moveList* createMoveList();
undoRedoNode* createNewUndoRedoNode();

void insertGameMove(moveList *list, gameMove *move);
void insertUndoRedoNodeWithMove(undoRedoList *list, gameMove *move);
void insertUndoRedoNodeWithList(undoRedoList *list, moveList *movesList);

void emptyUndoRedoList(undoRedoList *list);
void deleteLastUndoRedo(undoRedoList *urlist);
void deleteCurrentToLast(undoRedoList *list);
void freeMoveList(moveList *list);
void deleteLastMove(moveList *list);

#endif /*SUDOKU_STRUCTURES_H*/
