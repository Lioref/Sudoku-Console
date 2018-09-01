# Sudoku-console
The project is an implementation of an (m\*n)\*(m\*n) Sudoku that can be run via the console.
The sudoku boards are solved using the Gurobi solver api, presenting the sudoku as a linear programming problem. 
Thus, a Gurobi license is needed to download the gurobi library from the [Gurobi website](http://www.gurobi.com/index), then compile and run the game.

The project was the final coding assignment for a university course in C language.

## THE GAME has 3 modes:
Init<br/>
Edit<br/>
Solve<br/>

## The user commands that can be used to play are: 
1.  Solve X - Loads a puzzle from a file with path X in solve mode
2.  Edit X - Loads a puzzle from a file with path X in edit mode, if entered with no parameters, generates an 9*9 empty game board
3.  Mark_errors X - Gets a 0/1 as parameter, if 1: marks errors when printing board with *, else, doesn't mark the errors when printing
4.  Print_board - Prints the Sudoku board
5.  Set X Y Z - Sets cell X Y on the board to the value Z
6.  validate - Checks that board is solvable using the Gurobi solver
7.  generate X Y - Generates a puzzle on an empty Sudoku board, leaving Y hints on the board
8.  undo - undoes the last user move
9.  redo - redoes the last user move
10. save X - saves the current game board to a file with path X
11. hint X Y - gives a valid legal value that can be set to the cell X Y
12. Num_solutions - Calculates the number of solutions available for the current board
13. Autofill - Automatically fills cells that have only one possible legal value 
14. Reset - Resets board to it's original state when loaded with solve/edit commands
15. Exit - Exit the game

## GAME FILE FORMATS
The sudoku board can be saved in a txt file, with the first line consisting of the rows (m) and columns (n) variables, separated by a space.<br/>
Then, the board is saved with the numbers separated by a space:<br/>
	Empty cells are saved as 0<br/>
	Fixed cells are saved with *<br/>
for example:<br/>
3 2<br/>
0 1 0 0 0 0<br/>
0 0 0 0 0 0<br/>
0 0 0 0 0 0<br/>
0 0 0 0 0 0<br/>
0 0 0 0 *6 0<br/>
0 0 0 0 0 0<br/>
Represents a board where m=3, n=2, meaning each block is 3 rows * 2 columns.<br/>
Almost all cells are empty except for cell <2,1> which has the value 1, and cell <5,5> which has the fixed value 6.

## REMARKS
1. When referring to cell X Y on the game board - X is the columns argument and Y is the rows argument (like a coordinates system)
2. If a new solve/edit command is issued before saving, all changes will be lost
3. A sudoku board that is not of default size 9*9 can be created only by loading from a txt file in the format specified above
