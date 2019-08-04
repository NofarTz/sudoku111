//
// Created by Rubit on 30/07/2019.
//

#include "Solver.h"
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "MainAux.h"
#include "gurobi_c.h"

void initializeObj(int *obj,int sizeOfArray){
    int i;
    for( i=0; i < sizeOfArray; i++){
        obj[i] = 0;
    }
    return;
}
void initializeVtype(char *vtype,int sizeOfArray){
    /* other options: GRB_INTEGER, GRB_CONTINUOUS */
    int i;
    for( i=0; i < sizeOfArray; i++){
        vtype[i] = GRB_BINARY;
    }
    return;
}
int solveUsingILP(Board *b){
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       error = 0;
    double    *sol;
    int       *obj;
    char      *vtype;

    int       *ind;
    double    *val;
    int       optimstatus;
    double    objval;

    int constrainsCounter =0;
    int i,j,k,m,value,sizeOfConstrain,counter;
    int size = b->m * b->n;
    int numOfVar;
    /*
     * need to allocate it dynamically!!!
     * initialize to 1's
     */
    int optionsForVariables[9][9][9];


    /* Create environment - log file is mip1.log */
    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Create an empty model named "mip1" */
    error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Add variables */
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            value = b->board[i][j].value;
            if(value ==0){
                continue;
            }
            for(k=0;k<size;k++){
                optionsForVariables[k][j][value-1]=0;

            }
            for(k=0;k<size;k++){
                optionsForVariables[i][k][value-1]=0;

            }
            for(k=0;k<size;k++){
                optionsForVariables[i][j][k] =0;
            }
            /*
             * block
             */
        }
    }
    numOfVar = countNonZeroValues(optionsForVariables);
    sol = calloc(numOfVar, sizeof(double));
    obj = calloc(numOfVar, sizeof(int));
    vtype = calloc(numOfVar, sizeof(char));
    if(callocFailed("solveUsingILP",vtype) ||  callocFailed("solveUsingILP",obj) || callocFailed("solveUsingILP",sol) ){
        return 0;
    }
    initializeObj(obj,numOfVar);
    initializeVtype(vtype,numOfVar);
    numberTheVariables(optionsForVariables);

    /* add variables to model */
    error = GRBaddvars(model, numOfVar, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
        printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Change objective sense to maximization */
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* update the model - to integrate new variables */

    error = GRBupdatemodel(model);
    if (error) {
        printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /*
     * add constraints
     */

    /*
     * each position in the board contains exactly one value!
     */

    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            sizeOfConstrain = numOfNonZeroValues(optionsForVariables[i][j]);
            /*
             * >0 or >1???
             */
            if(sizeOfConstrain>0){
                counter =0;
                ind = calloc(sizeOfConstrain, sizeof(int));
                val = calloc(sizeOfConstrain, sizeof(int));
                if(callocFailed("",ind) || callocFailed("",val)){
                    return 0;
                }
                for(m=0;m<size;m++){
                    if(optionsForVariables[i][j][m] != 0){
                        ind[counter] = optionsForVariables[i][j][m];
                        counter +=1;
                    }
                }
                setToOnes(val);
                error = GRBaddconstr(model, sizeOfConstrain, ind, val, GRB_LESS_EQUAL, 1, constrainsCounter+ '0');
                constrainsCounter+=1;
                if (error) {
                    printf("ERROR %d %dst GRBaddconstr(): %s\n", error,constrainsCounter, GRBgeterrormsg(env));
                    return -1;
                }

                error = GRBaddconstr(model, sizeOfConstrain, ind, val, GRB_GREATER_EQUAL, 1, constrainsCounter+ '0');
                constrainsCounter+=1;
                if (error) {
                    printf("ERROR %d %dst GRBaddconstr(): %s\n", error,constrainsCounter, GRBgeterrormsg(env));
                    return -1;
                }

            }
        }
    }


    /* Second constraint: x + y >= 1 */
    ind[0] = 0; ind[1] = 1;
    val[0] = 1; val[1] = 1;

    /* add constraint to model - note size 2 + operator GRB_GREATER_EQUAL */
    /* -- equation value (1.0) + unique constraint name */
    error = GRBaddconstr(model, 2, ind, val, GRB_GREATER_EQUAL, 1.0, "c1");
    if (error) {
        printf("ERROR %d 2nd GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
        printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* Get solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* get the solution - the assignment to each variable */
    /* 3-- number of variables, the size of "sol" should match */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, 3, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    /* print results */
    printf("\nOptimization complete\n");

    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {
        printf("Optimal objective: %.4e\n", objval);
        printf("  x=%.2f, y=%.2f, z=%.2f\n", sol[0], sol[1], sol[2]);
    }
        /* no solution found */
    else if (optimstatus == GRB_INF_OR_UNBD) {
        printf("Model is infeasible or unbounded\n");
    }
        /* error or calculation stopped */
    else {
        printf("Optimization was stopped early\n");
    }

    /* IMPORTANT !!! - Free model and environment */
    GRBfreemodel(model);
    GRBfreeenv(env);

    return 0;
}
