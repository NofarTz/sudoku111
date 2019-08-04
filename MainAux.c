
#include "MainAux.h"
#include "SPBufferset.h"
#include <stdlib.h>


/*
 * gets an enum error and calls the right function
 */
void ErrorPrinting(enum Errors error, char funcName[]) {
    SP_BUFF_SET()
            ;
    switch (error) {
        case MEMORYALLOCATION:
            printf("Error: %s has failed\n", funcName);
            break;
        case INVALIDCELLSNUMBER:
            printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
            break;
        case FEOF:
            printf("Exiting...\n");

            break;
        case FIXEDCELL:
            printf("Error: cell is fixed\n");
            break;
        case INVALIDVALUE:
            printf("Error: value is invalid\n");
            break;
        case INVALIDCOMMAND:
            printf("Error: invalid command\n");
            break;
        case WRITEFILE:
            printf("Error in writing to file \n");
            break;
        case READFILE:
            printf("Error in reading from file\n");
            break;
        case INVALIDBOARD:
            printf("The file does not contain a valid board");
    }

}

void print_board(Board *b, int m, int n){
    int i=0,row=0,col=0;
    SP_BUFF_SET()
    ;
    if((*b).mode==0){
        /*
         * vat to do??
         */
    }
    /*
     * maybe it's safer if we unmark the board first?
     */
    if((*b).mode == 1 || ((*b).mode == 2 && (*b).markErrors ==1) ) {
        markBoard(b);
    }
    for ( i = 0; i < 4*(m*n)+m+1; i++) {
        printf("-");
    }
    for ( row = 0; row < m*n; row++) {
        printf("\n");
        printf("|");
        for ( col = 0; col < m*n; col++) {
            printf(" ");
            if (isFixed(b, row, col)) {
                printf("%2d.", getValue(b, row, col));
            }
            else if(isMarked(b, row, col)){
                printf("%2d*", getValue(b, row, col));
            }
            else {
                if (getValue(b, row, col) == 0) {
                    printf("  ");
                } else {
                    printf(" %2d", getValue(b, row, col));
                }
            }
            if (col % 3 == 2) {
                printf("|");
            }
        }
        if (row % 3 == 2) {
            printf("\n");
            for ( i = 0; i < 4*(m*n)+m+1; i++) {
                printf("-");
            }

        }
    }
    printf("\n");
    unMarkBoard(b);
}

int callocFailed(char funcName[], void* arr) {
    if (arr == NULL ) {
        ErrorPrinting(MEMORYALLOCATION, funcName);
        return 1;
    }
    return 0;
}

