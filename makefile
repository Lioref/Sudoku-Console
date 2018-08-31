CC = gcc
OBJS = main.o Aux.o Game.o Parser.o Solver.o ReadWrite.o LinkedLists.o Stack.o 
EXEC = sudoku-console
COMP_FLAG = -ansi -O3 -Wall -Wextra \
-Werror -pedantic-errors
GUROBI_COMP = -I/Library/gurobi801/mac64/include #the local path to the include folder of the gurobi header file
#/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/Library/gurobi801/mac64/lib -lgurobi80 #the local path to the gurobi library file
#/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
all: sudoku-console
main.o: main.c Aux.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAG) -c $*.c
LinkedList.o: LinkedLists.c LinkedLists.h 
	$(CC) $(COMP_FLAG) -c $*.c
Aux.o: Aux.c Aux.h Game.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h Parser.h ReadWrite.h Solver.h LinkedLists.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c Solver.h Stack.h Aux.h LinkedLists.h gurobi_c.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
ReadWrite.o: ReadWrite.c ReadWrite.h LinkedLists.h Aux.h
	$(CC) $(COMP_FLAG) -c $*.c


clean:
	rm -f $(OBJS) $(EXEC)