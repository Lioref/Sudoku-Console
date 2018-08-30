#include "Solver.h"

/**
 * exits the program, printing the given name of the function.
 * @param functionName
 */
void exitErrorSolver(char *functionName) {
    printf("Error: %s has failed\n", functionName);
    exit(1);
}

/**
 * given the current column, gives the next column in the left-right top-down order
 * @param y column
 * @param m rows in board box
 * @param n cols in board box
 * @return
 */
int nextCol(int y, int m, int n) {
    int N;
    N = m*n;
    if (y==(N-1)) {
        return 0;
    } else {
        return y+1;
    }
}

/**
 * given the current row and column, gives the next row in the left-right top-down order
 * @param y column
 * @param m rows in board box
 * @param n cols in board box
 * @return
 */
int nextRow(int x, int y, int m, int n) {
    int N;
    N = m*n;
    if (y==(N-1)) {
        return x+1;
    } else {
        return x;
    }
}

/**
 * checks if num is a valid input for cell in row x
 * @param x row
 * @param y col
 * @param m rows in board box
 * @param n cols in board box
 * @param num number of validity check
 * @param auxBoard pointer to auxillary board
 * @return boolean
 */
int isValidRow(int x, int y, int m, int n, int num, int **auxBoard) {
    int i,N;
    N = m*n;
    /* make sure num does not appear in row x */
    for (i=0; i<N; i++) {
        if (auxBoard[x][i]==num && i!=y) { /* if num is allready in x,y this is ok */
            return 0;
        }
    }
    return 1; /* only reached if num is not in the row */
}

/**
 * checks if num is a valid input for cell in col y
 * @param x row
 * @param y col
 * @param m rows in board box
 * @param n cols in board box
 * @param num number of validity check
 * @param auxBoard pointer to auxillary board
 * @return boolean
 */
int isValidCol(int x, int y, int m, int n, int num, int **auxBoard) {
    int i,N;
    N = m*n;
    /* */
    for (i=0; i<N; i++) {
        if (auxBoard[i][y]==num && i!=x) { /* if num is in x,y this is ok */
            return 0;
        }
    }
    return 1;  /* only reached if num is not in the col */
}

/**
 * checks if num is a valid input for cell in box of x,y
 * @param x row
 * @param y col
 * @param m rows in board box
 * @param n cols in board box
 * @param num number for validity check
 * @param auxBoard pointer to auxillary board
 * @return boolean
 */
int isValidBox(int x, int y, int num, int m, int n, int **auxBoard) {
    int boxRowIndex,boxColIndex,i,j;
    boxRowIndex = m*(x/m);
    boxColIndex = n*(y/n);
    /* start at top left corner of box, and iterate through box values */
    for (i=boxRowIndex; i<(boxRowIndex+m); i++) {
        for (j=boxColIndex; j<(boxColIndex+n); j++) {
            assert(auxBoard[i][j]<=(m*n));
            if (auxBoard[i][j]==num) {
                return 0;
            }
        }
    }
    return 1; /* only reached if num is not in the box */

}

/**
 * wrapper function, checks validity of num for cell x,y with regard to row, col and box
 * @param x row
 * @param y col
 * @param num number for validity check
 * @param m rows in board box
 * @param n cols in board box
 * @param auxBoard pointer to int matrix
 * @return
 */
int isValidNum(int x, int y, int num, int m, int n, int **auxBoard) {
    int rowValidity,colValidity,boxValidity;
    rowValidity = isValidRow(x, y, m, n, num, auxBoard);
    colValidity = isValidCol(x, y, m, n, num, auxBoard);
    boxValidity = isValidBox(x, y, num, m, n, auxBoard);
    return rowValidity*colValidity*boxValidity;
}

/**
 * checks if board has solition, 1 if has, 0 if not feasable, -1 on error
 * @param gameBoard pointer to cell matrix
 * @param m rows in board box
 * @param n cols in board box
 * @return boolean
 */
int hasSolution(cell **gameBoard, int m, int n) {
    int opts;
    int **auxBoard;
    auxBoard = createAuxBoard(m, n);
    opts = solveBoard(gameBoard, auxBoard, m, n, 0);
    freeAuxBoard(auxBoard, m, n);
    if (opts==2) {
        return 1;
    } else if (opts==3) {
        return 0;
    } else {
        return -1;
    }
}

/**
 * counts the number of solutions a board has
 * @param auxBoard int matrix
 * @param m rows in box
 * @param n cols in box
 * @return number of solutions (long)
 */
long exhaustiveBacktracking(int **auxBoard, int m, int n) {
    int num, cnt, res, currentStage, row, col, N; /* Return variable */
    stack *stack;
    callState *currentState, *nextState;
    /* Init variables */
    N = (m*n);
    res = 0;
    stack = createStack();
    if (!stack) {
        exitErrorSolver("exhaustiveBacktracking");
    }
    currentState = createCallState(0, 0, 1, 0);
    if (!currentState) {
        exitErrorSolver("exhaustiveBacktracking");
    }
    push(stack, currentState);
    /* Main loop */
    while(!isEmpty(stack)) {
        /* Get the next call */
        currentState = pop(stack);
        currentStage = currentState->stage;
        row = currentState->x;
        col = currentState->y;
        assert(col >= 0);
        assert(col < N);
        assert(row >= 0);
        assert(row <= N);
        if (currentStage == 1) {
            /* Out of bounds base case */
            if (row > (N-1)) {
                res += 1;
                free(currentState);
                continue;
            }
            /* Skip over full cell case */
            if (auxBoard[row][col] != 0) {
                nextState = createCallState(nextRow(row, col, m, n), nextCol(col, m, n), 1, 0);
                if (!nextState) {
                    exitErrorSolver("exhaustiveBacktracking");
                }
                push(stack, nextState);
                free(currentState);
                continue;
            }
            else {
                nextState = createCallState(row, col, 2, 0);
                push(stack, nextState);
                free(currentState);
                continue;
            }
        }
        /* cell is in bounds and empty, check possible cell values */
        else if (currentStage == 2) {
            cnt = 0;
            for (num = (currentState->num+1) ; num <= N ; num++) {
                if(isValidNum(row, col, num, m, n, auxBoard)) {
                    cnt++;
                    /* Set first valid number that hasn't been exhausted yet */
                    auxBoard[row][col] = num;
                    nextState = createCallState(row, col, 2, num);
                    if (!nextState) {
                        exitErrorSolver("exhaustiveBacktracking");
                    }
                    push(stack, nextState);
                    /* Continue to the next cell */
                    nextState = createCallState(nextRow(row, col, m, n), nextCol(col, m, n), 1, 0);
                    if (!nextState) {
                        exitErrorSolver("exhaustiveBacktracking");
                    }
                    push(stack, nextState);
                    break;
                }
            }
            if (cnt == 0) {
                /* Exhausted all possible values for board cell */
                auxBoard[row][col] = 0;
            }
            free(currentState);
            continue;
        }
    }
    freeStack(stack);
    return res;
}

/**
 * convert 3D coordinates in a N**3 cube to vector index
 * @param i row
 * @param j col
 * @param k depth
 * @param m rows in board box
 * @param n cols in board box
 * @return array index (integer)
 */
int matCoords2ArrayCoords(int i, int j, int k, int m, int n) {
    int N = m*n;
    int N2 = (N*N);
    return (N2)*i + (N)*j + k;
}

/**
 * convert vector index to 3D coordinates in a N**3 cube
 * @param ind array index
 * @param m rows in board box
 * @param n cols in board box
 * @param coords pointer to array of ints of size 3
 */
void arrayCoords2MatCoords(int ind, int m, int n, int *coords) {
    int N;
    N = m*n;
    coords[0] = ind/(N*N);
    coords[1] = (ind % (N*N))/N;
    coords[2] = (ind % (N*N))%N;
}

/**
 * copies values from gurobi solution vector to auxBoard
 * @param solutionVec solution to sudoku board in array
 * @param auxBoard int matrix
 * @param m rows in board box
 * @param n cols in board box
 */
void solution2AuxBoard(double *solutionVec, int **auxBoard, int m, int n) {
    int i, N, numVars;
    int *coords;
    coords = calloc(3, sizeof(int));
    if (!coords) {
        exitErrorSolver("solution2AuxBoard");
    }
    N = m*n;
    numVars = (int) pow(N, 3);
    for (i=0; i<numVars; i++) {
        if (solutionVec[i]!=0) {
            arrayCoords2MatCoords(i, m, n, coords);
            auxBoard[coords[0]][coords[1]] = coords[2]+1;
        }
    }
    free(coords);
}

/**
 * calculates and saves coordinates of sudoku box in vector indices, for box constraints
 * @param minRow top row of the box
 * @param minCol leftmost column of the box
 * @param depth
 * @param m rows in box
 * @param n cols in box
 * @param indArr pointer to array of indices
 */
void getBoxIndices(int minRow, int minCol, int depth, int m, int n, int *indArr) {
    int r,c,cnt;
    cnt = 0;
    for (r=minRow; r<minRow+m; r++) {
        for (c=minCol; c<minCol+n; c++) {
            indArr[cnt] = matCoords2ArrayCoords(r, c, depth, m, n);
            cnt++;
        }
    }
}

/**
 * free optimization structures
 * @param env gurobi
 * @param model gurobi
 * @param varTypes array of variable types
 * @param coeffs array of coefficients for the variables
 * @param constrInds array of constraint indices
 * @param constrVals array of constraint variable values
 * @param solutionVec array for returned solution
 */
void freeOptimizationStruct(GRBenv *env, GRBmodel *model, double *solutionVec) {
    GRBfreemodel(model);
    GRBfreeenv(env);
    free(solutionVec);
}

/**
 * adds binary variables to model, returns 1 if succesfull, 0 otherwise
 * @param env gurobi environment
 * @param model model to add variables to
 * @param numVars number of variables to add to the model
 * @param v verbosity
 * @return success integer
 */
int addVarsToModel(GRBenv *env, GRBmodel *model, int numVars, int v) {
    /* declare vars */
    int *coeffs;
    char *varTypes;
    int i,error;

    /* initialize  coeff and variable type vectors */
    coeffs = calloc((size_t ) numVars, sizeof(int));
    if (!coeffs) {
        exitErrorSolver("addVarsToModel");
    }
    varTypes = calloc((size_t ) numVars, sizeof(char));
    if (!varTypes) {
        exitErrorSolver("addVarsToModel");
    }
    for (i=0; i<numVars; i++) {
        coeffs[i] = 1;
        varTypes[i] = GRB_BINARY;
    }
    error = GRBaddvars(model, numVars, 0, NULL, NULL, NULL, NULL, NULL, NULL, varTypes, NULL);
    if (error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        /* free resources before return */
        free(coeffs);
        free(varTypes);
        return 0;
    }
    /* update the model - to integrate new variables */
    error = GRBupdatemodel(model);
    if (error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        /* free resources before return */
        free(coeffs);
        free(varTypes);
        return 0;
    }
    free(coeffs);
    free(varTypes);
    return 1;
}

/**
 * adds constraints to model, enforcing a single value per sudoku cell constraint
 * @param env gurobi environment
 * @param model to add constraints to
 * @param v verbosity
 * @param m rows in sudoku box
 * @param n cols in sudoku box
 * @return success integer, 1 if success
 */
int addSingeValConstraintsToModel(GRBenv *env, GRBmodel *model, int v, int m, int n) {
    /* declare vars */
    int error,i,j,k,N;
    int *constrInds;
    double *constrVals;
    /* initialize */
    N = m*n;
    constrInds = calloc((size_t) N, sizeof(int));
    if (!constrInds) {
        exitErrorSolver("addSingleValConstraintsToModel");
        return -1;
    }
    constrVals = calloc((size_t) N, sizeof(double));
    if (!constrVals) {
        exitErrorSolver("addSingleValConstraintsToModel");
        return -1;
    }
    /* add constraints */
    /* ones array for constraint coefficients */
    for (i=0; i<N; i++) {
        constrVals[i] = (double) 1;
    }
    /* single value constraints */
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            /* variable indices */
            for (k=0; k<N; k++) {constrInds[k] = matCoords2ArrayCoords(i, j, k, m, n);}
            /* add constr */
            error = GRBaddconstr(model, N, constrInds, constrVals, GRB_EQUAL, 1, NULL);
            if (error) {
                if (v) {printf("%s\n", GRBgeterrormsg(env));}
                free(constrInds);
                free(constrVals);
                return 0;
            }
        }
    }
    /* free and return */
    free(constrInds);
    free(constrVals);
    return 1;

}

/**
 * add constraints to model enforcing only a single appearance of each number in a row
 * @param env gurobi env
 * @param model model to add constraints to
 * @param v verbosity
 * @param m rows in sudoku box
 * @param n cols in sudoku box
 * @return success integer, 1 if success, 0 else
 */
int addRowConstraintsToModel(GRBenv *env, GRBmodel *model, int v, int m, int n) {
    /* declare */
    int i,j,k,error,N;
    int *constrInds;
    double *constrVals;
    /* initialize */
    N = m*n;
    constrInds = calloc((size_t) N, sizeof(int));
    if (!constrInds) {
        exitErrorSolver("addRowConstraintsToModel");
        return -1;
    }
    constrVals = calloc((size_t) N, sizeof(double));
    if (!constrVals) {
        exitErrorSolver("addRowConstraintsToModel");
        return -1;
    }
    for (i=0; i<N; i++) {
        constrVals[i] = (double) 1;
    }
    /* row constraints */
    for (i=0; i<N; i++) {
        for (k=0; k<N; k++) {
            /* update array of indices for row constraint */
            for (j=0; j<N; j++) {constrInds[j] = matCoords2ArrayCoords(i, j, k, m, n);}
            /* add constraint */
            error = GRBaddconstr(model, N, constrInds, constrVals, GRB_EQUAL, 1, NULL);
            if (error) {
                if (v) {printf("%s\n", GRBgeterrormsg(env));}
                free(constrInds);
                free(constrVals);
                return 0;
            }
        }
    }
    free(constrInds);
    free(constrVals);
    return 1;
}

/**
 * add constraints to model enforcing only a single appearance of each number in a col
 * @param env gurobi env
 * @param model to add constraints to
 * @param v verbosity
 * @param m rows in sudoku box
 * @param n cols in sudoku box
 * @return success integer
 */
int addColConstraintsToModel(GRBenv *env, GRBmodel *model, int v, int m, int n) {
    /* declerations */
    int i,j,k,N,error;
    int *constrInds;
    double *constrVals;
    /* initialize */
    N = m*n;
    constrInds = calloc((size_t) N, sizeof(int));
    if (!constrInds) {
        exitErrorSolver("addColConstraintsToModel");
        return -1;
    }
    constrVals = calloc((size_t) N, sizeof(double));
    if (!constrVals) {
        exitErrorSolver("addColConstraintsToModel");
        return -1;
    }
    for (i=0; i<N; i++) {
        constrVals[i] = (double) 1;
    }
    /* col constraints */
    for (j=0; j<N; j++) {
        for (k=0; k<N; k++) {
            /* update array of indices for column constraints */
            for (i=0; i<N; i++) {constrInds[i] = matCoords2ArrayCoords(i, j, k, m, n);}
            error = GRBaddconstr(model, N, constrInds, constrVals, GRB_EQUAL, 1, NULL);
            if (error) {
                if (v) {printf("%s\n", GRBgeterrormsg(env));}
                free(constrInds);
                free(constrVals);
                return 0;
            }
        }
    }
    free(constrInds);
    free(constrVals);
    return 1;

}

/**
 * add constraints to model enforcing only a single appearance of each number in a box
 * @param env gurobi env
 * @param model to add constraints to
 * @param v verbosity
 * @param m rows in sudoku box
 * @param n cols in sudoku box
 * @return success integer (1 if success, -1 if system error, 0 if gurobi error
 */
int addBoxConstraintsToModel(GRBenv *env, GRBmodel *model, int v, int m, int n) {
    /* declerations */
    int i,j,k,N,error;
    int *constrInds;
    double *constrVals;
    /* initialize */
    N = m*n;
    constrInds = calloc((size_t) N, sizeof(int));
    if (!constrInds) {
        exitErrorSolver("addBoxConstraintsToModel");
        return -1;
    }
    constrVals = calloc((size_t) N, sizeof(double));
    if (!constrVals) {
        exitErrorSolver("addBoxConstraintsToModel");
        return -1;
    }
    for (i=0; i<N; i++) {
        constrVals[i] = (double) 1;
    }
    /* box constraints */
    for (i=0; i<n; i++) {
        for (j=0; j<m; j++) {
            for (k=0; k<N; k++) {
                getBoxIndices(m*i, n*j, k, m, n, constrInds);
                error = GRBaddconstr(model, N, constrInds, constrVals, GRB_EQUAL, 1, NULL);
                if (error) {
                    if (v) {printf("%s\n", GRBgeterrormsg(env));}
                    free(constrInds);
                    free(constrVals);
                    return 0;
                }
            }
        }
    }
    free(constrInds);
    free(constrVals);
    return 1;
}

/**
 * add constraint for each fixed value in gameBoard
 * @param env gurobi env
 * @param model to add constraints to
 * @param v verbosity
 * @param m rows in sudoku box
 * @param n cols in sudoku box
 * @param gameBoard cell matrix
 * @return success integer
 */
int addFixedCellConstraintsToModel(GRBenv *env, GRBmodel *model, int v, int m, int n, cell **gameBoard) {
    /* declerations */
    int i,j,N,val,ind,error;
    double cval;
    /* constraints of fixed values in the board */
    N = m*n;
    cval = 1; /* coefficient of new contraint is always 1 */
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            val = gameBoard[i][j].value;
            if (val!=0) {
                ind = matCoords2ArrayCoords(i, j, val-1, m, n);
                error = GRBaddconstr(model, 1, &ind, &cval, GRB_EQUAL, 1, NULL);
                if (error) {
                    if (v) {printf("%s\n", GRBgeterrormsg(env));}
                    return 0;
                }
            }
        }
    }
    return 1;
}

/**
 * uses gurobi to solve board, returns optim status, and populates auxBoard with solution. returns -1 on error
 * @param gameBoard cell matrix
 * @param auxBoard  int matrix
 * @param m rows in board box
 * @param n cols in board box
 * @param v verbosity, if 1 it prints optmization associated messages
 * @return gurobi optimization status (integer)
 */
int solveBoard(cell **gameBoard, int **auxBoard, int m, int n, int v) {
    /* declare variables */
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       N,nv; /* dimension variables */
    int       error = 0;
    int       optimstatus;
    double    *solutionVec; /* array used to save solution */

    /* row and column length + number of required vars */
    N = m*n;
    nv = (int) pow(N, 3);
    /* create solution vector */
    solutionVec = calloc((size_t) nv, sizeof(double));
    if (!solutionVec) {
        exitErrorSolver("solveBoard");
    }
    /* Create environment - log file is mip1.log */
    error = GRBloadenv(&env, "sdk1.log");
    if (error && v) {
        printf("%s\n", GRBgeterrormsg(env));
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* toggle console logs */
    if (!v) {GRBsetintparam(env,GRB_INT_PAR_LOGTOCONSOLE,0);}

    /* Create an empty model named "sdk1" */
    error = GRBnewmodel(env, &model, "sdk1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }

    /* Add variables */
    error = addVarsToModel(env, model, nv, v);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }

    /* add single value constraints */
    error = addSingeValConstraintsToModel(env, model, v, m, n);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* row constraints */
    error = addRowConstraintsToModel(env, model, v, m, n);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* col constraints */
    error = addColConstraintsToModel(env, model, v, m, n);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* box constraints */
    error = addBoxConstraintsToModel(env, model, v, m, n);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* constraints of fixed values in the board */
    error = addFixedCellConstraintsToModel(env, model, v, m, n, gameBoard);
    if (!error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* Get solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        if (v) {printf("%s\n", GRBgeterrormsg(env));}
        freeOptimizationStruct(env, model, solutionVec);
        return -1;
    }
    /* if problem is not feasable, return the status and free all structs */
    if (optimstatus!=2) {
        freeOptimizationStruct(env, model, solutionVec);
        return optimstatus;
    } else {
        /* get the solution - the assignment to each variable */
        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, nv, solutionVec);
        if (error) {
            if (v) {printf("%s\n", GRBgeterrormsg(env));}
            freeOptimizationStruct(env, model, solutionVec);
            return -1;
        }
        /* copy solution to auxBoard */
        solution2AuxBoard(solutionVec, auxBoard, m, n);
    }
    freeOptimizationStruct(env, model, solutionVec);
    return optimstatus;
}

