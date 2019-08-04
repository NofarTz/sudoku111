
#include "Game.h"
#include "MainAux.h"
#include "SPBufferset.h"
#include <stdio.h>
#include <stdlib.h>
/*
 * returns 1 if the board is filled with numbers in range (1,nm)
 */
int boardIsFilled(Board *gameBoard) {
    int i =0;
    int j=0;
    int m = gameBoard->m;
    int n = gameBoard->n;
    for ( i = 0; i < m * n; i++) {
        for ( j = 0; j < m * n; j++) {
            if ((*gameBoard).board[i][j].value == 0) {
                return 0;
            }
        }
    }
    (*gameBoard).isSolved = 1;
    return 1;
}
/*
 * marks every cell that has a conflict with cell row,col
 * returns 1 if there exists an erroneous cell
 */
int markErrors(Board *b, int row, int col) {
    int hasErrors =0;
    Cell **board = (*b).board;
    int i=0,j=0,startRow=0,startCol=0;
    int m = b->m;
    int n = b->n;
    int value = board[row][col].value;
    /*
     * if the value is 0 - it's never an error
     */
    if(value==0){
        return 0;
    }
    /*
     * mark every identical value in row
     */
    for (i = 0; i < m * n; i++) {
        if (i != col && board[row][i].value == value) {
            board[row][i].marked = 1;
            board[row][col].marked = 1;
            hasErrors =1;
        }
    }

    /*
     * mark every identical value in col
     */
    for (j = 0; j < m * n; j++) {
        if (j != row && board[j][col].value == value) {
            board[j][col].marked = 1;
            board[row][col].marked = 1;
            hasErrors =1;
        }
    }

    startRow = row - row % m;
    startCol = col - col % n;
    /*
     * mark every identical value in square
     */
    for ( i = startRow; i < startRow + m; i++) {
        for ( j = startCol; j < startCol + n; j++) {
            if (i != row && j != col && board[i][j].value == value) {
                board[i][j].marked = 1;
                board[row][col].marked = 1;
                hasErrors =1;
            }
        }
    }
    return hasErrors;

}
/*
 * marks every cell with an error.
 * returns 1 if there exists an erroneous cell
 */
int markBoard(Board *b) {
    int i, j;
    int boardIsErroneous;
    int m = b->m;
    int n = b->n;
    for (i = 0; i < m*n; i++) {
        for (j = 0; j < m*n; j++) {
            if(markErrors(b, i, j)==1){
                boardIsErroneous =1;
            }
        }
    }
    return boardIsErroneous;
}


void unMarkBoard(Board *b){
    int i,j;
    int m = b->m;
    int n = b->n;
    for( i=0;i<m*n;i++){
        for(j=0;j<m*n;j++){
            b->board[i][j].marked=0;
                }
            }

}
/*
 * returns 1 if the board is erroneous
 */
int isErroneous(Board *b){
    int i=markBoard(b);
    unMarkBoard(b);
    return i;

}
/*
 * sets the mark_errors field to x
 */
void mark_errors(Board *b, int x){
    (*b).markErrors = x;
}
/*
 * sets the value in position row,col to the value that was given
*/
void set(int x, int y, int z, Board *gameBoard,int m,int n) {
    SP_BUFF_SET();

     /* if the game is solved- set is an invalid command and we will return an error */

    if ((*gameBoard).mode == 0) {
        ErrorPrinting(INVALIDCOMMAND, "set");
        return;
    }
    if ((*gameBoard).mode == 2 && (*gameBoard).board[x][y].fixed) {
        ErrorPrinting(FIXEDCELL, "set");
        return;
    }
    (*gameBoard).board[x][y].value = z;

    print_board(gameBoard, m, n);
    if ((*gameBoard).mode==2 && boardIsFilled(gameBoard)) {
        if(isErroneous(gameBoard)){
            printf("The solution is erroneous");
            return;
        }
        printf("Puzzle solved successfully\n");
        (*gameBoard).mode =0;
    }

    return;
}

/*
 * sets the value in position row,col to the value that was given

void set(int x, int y, int z, Board *gameBoard,int m,int n) {
    *
     * added only this line (need to update ellie)
     *
    struct Action* newAction= (struct Action*)calloc(1, sizeof(struct Action*));
    int arr[3];
    SP_BUFF_SET();
    *
     * if the game is solved- set is an invalid command and we will return an error
     *
    if ((*gameBoard).mode == 0) {
        ErrorPrinting(INVALIDCOMMAND, "set");
        return;
    }

    if ((*gameBoard).mode == 2 && (*gameBoard).board[x][y].fixed) {
        ErrorPrinting(FIXEDCELL, "set");
        return;
    }
    arr[0]=x;
    arr[1]=y;
    arr[2]= getValue(gameBoard, x, y);
    newAction->command=SET;
    newAction->arg=arr;
    newAction->prev=gameBoard->actionP;
    *need to free memory from p.next to the end of the list *
    ((gameBoard)->actionP)->next=newAction;
    gameBoard->actionP=newAction;

    (*gameBoard).board[x][y].value = z;


    print_board(gameBoard, m, n);
    if ((*gameBoard).mode==2 && boardIsFilled(gameBoard,m,n)) {
        if(markBoard(gameBoard,m,n) == 1){
            printf("The solution is erroneous");
            unMarkBoard(gameBoard,m,n);
            return;
        }
        printf("Puzzle solved successfully\n");
        (*gameBoard).mode =0;
    }

    return;
}
*/
int save(Board *b, char *path){
    int i,j,value;
    int m = b->m;
    int n = b->n;
    FILE *fptr_out = fopen(path,"w");
    if((*b).mode==0 || fptr_out == NULL){
        ErrorPrinting(INVALIDCOMMAND,"save");
        fclose(fptr_out);
        return 0;
    }
    unMarkBoard(b);
    if((*b).mode == 1 || ((*b).mode == 2 && (*b).markErrors ==1) ) {
        markBoard(b);
    }
    if(fprintf(fptr_out,"%d",m) < 0 ||
    fprintf(fptr_out,"%c",' ') <0||
    fprintf(fptr_out,"%d",n)<0 ||
    fprintf(fptr_out,"%c",'\n')<0){
        ErrorPrinting(WRITEFILE,"save");
        fclose(fptr_out);
        return 0;
    }

    for(i=0;i<m*n;i++){
        for(j=0;j<m*n;j++){
            value = (*b).board[i][j].value;
            if(fprintf(fptr_out,"%d",value) < 0 ||
            ((*b).board[i][j].fixed==1 && fprintf(fptr_out,"%c",'.') < 0) ||
            ((*b).board[i][j].marked==1 && fprintf(fptr_out,"%c",'*') < 0) ||
            (j!= m*n-1 && fprintf(fptr_out,"%c",' ') < 0)){
                ErrorPrinting(WRITEFILE,"save");
                fclose(fptr_out);
                return 0;
            }
        }
        if(fprintf(fptr_out,"%c",'\n') < 0){
            ErrorPrinting(WRITEFILE,"save");
            fclose(fptr_out);
            return 0;
        }
    }
    fclose(fptr_out);
    if(feof(fptr_out)){
        ErrorPrinting(WRITEFILE,"save");
        fclose(fptr_out);
        return 0;
    }
    fclose(fptr_out);
    return 1;
}
/*
 * gets a Board, and initializes the cell's array in it. the size of the array is determent bt b->m,b->n.
 */
int intializeEmptyBoard(Board* b) {
    int m = b->m;
    int n = b->n;
    /*
     * what should we intialize the mode to? and mark errors? and actions?
     */
    Cell **cells = (Cell**) calloc(m * n, sizeof(Cell*));
    int i =0;
    int j =0;
    b->isSolved =0;
    if(callocFailed("createEmptyBoard", cells)){
        free(cells);
        return 0;

    }
    for ( i = 0; i < m * n; i++) {
        cells[i] = (Cell*) calloc(m * n, sizeof(Cell));
        if(callocFailed("createEmptyBoard", cells[i])){
            for( j = 0; j <= i ; j++){
                free(cells[j]);

            }
            free(cells);
            return 0;
        }

    }
    /*
     * initializing the cell's fields (except the options and nunmOfOptions)
     */
    for(i=0;i<m*n;i++){
        for(j=0;j<m*n;j++){
            cells[i][j].value =0;
            cells[i][j].fixed = 0;
            cells[i][j].marked =0;
        }
    }
    (*b).board = cells;
    return 1;
}
void freeCellsInBoard(Board *b) {
    int m = b->m;
    int n = b->n;
    int i =0;
    for ( i = 0; i < m * n; i++) {
        free((*b).board[i]);
    }
    free((*b).board);
    b->m =0;
    b->n=0;
}

void promoteIndex(int *a,int *b,int m,int n){
    if((*b)== m*n-1){
        (*b) = 0;
        *a = *a + 1;
        return;
    }
    *b = *b +1;
    return;
}

int edit(Board *b, char *path) {
    int newM, newN, value;
    int i = 0;
    int j = 0;
    FILE *fptr_in;
    char character;
    /*
     * do we need to free more stuff??
     * (the mark errors field should remain the same as it was before
     * can this go wrong??
     * for example, what happens if b is null??
     * will this be problamatic if it is the first command of the game? do we already have a board to free?
     *
     *
     * another problem- if we discover half way through that the board is invalid- what should we do?
     * as for now- we loaded the "good half" of the board
     * */
    freeCellsInBoard(b);
    /*
     * setting the mode to edit mode
     */
    (*b).mode = 1;
    if (path == NULL) {
        b->m = 3;
        b->n = 3;
        intializeEmptyBoard(b);
        /*
         * vat to do?
         * return?
         */
    } else {
        fptr_in = fopen(path, "r");
        /*
         * if we cant open the file- print an error message
         */
        if (fptr_in == NULL) {
            ErrorPrinting(READFILE, "edit");
            fclose(fptr_in);
            return 0;
        }
        /*
         * if there are no two valid values for the board size- print an eror
         */
        if (fscanf(fptr_in, "%d %d", &newM, &newN) < 2 || newM <0 || newM <0) {
            ErrorPrinting(INVALIDBOARD,"edit");
            fclose(fptr_in);
            return 0;
        }
        b->m = newM;
        b->n = newN;
        intializeEmptyBoard(b);
        /*
         * reading the board values from the text file to the board
         */
        while ((i < newM*newN && j < newM*newN) && fscanf(fptr_in, "%d", &value) == 1) {
            if(value < 0 || value > newM*newN){
                ErrorPrinting(INVALIDBOARD,"edit");
                fclose(fptr_in);
                return 0;
            }
            character = fgetc(fptr_in);
            while (character != EOF) {
                if (character == ' ' || character == '\n' || character == '\r' || character == '\n') {
                    character = fgetc(fptr_in);
                    continue;
                }
                if (character == '.') {
                    b->board[i][j].fixed = 1;
                    break;
                } else if (character == '*') {
                    b->board[i][j].marked = 1;
                    break;
                } else {
                    fseek(fptr_in, -1, SEEK_CUR);
                    break;
                }
            }
            b->board[i][j].value = value;
            promoteIndex(&i, &j, newM,newN);
        }
        /*
         * if there are not enough values- print an error
         */
        if(i < newM && j < newN){
            ErrorPrinting(INVALIDBOARD,"edit");
            fclose(fptr_in);
            return 0;
        }
        character = fgetc(fptr_in);
        /*
         * if there are two many characters (that are not spaces)- print an error
         */
        while (character != EOF) {
            if (character != ' ' || character != '\n' || character != '\r' || character != '\n') {
                ErrorPrinting(INVALIDBOARD,"edit");
                fclose(fptr_in);
                return 0;
            }
            /*
             * why is it unreachable code??
             */
            character = fgetc(fptr_in);
        }
        fclose(fptr_in);
    }
    /*
     * we need to return the newM,newN (or maybe save it as a field of Board?)
     *
     *
     * we need to check that the board is not errounous
     */

    return 1;
}
void autofill(Board *gameBoard, int m, int n){
    /*
     * we need to free copy
     * we dont need to free newAction
     */
    Board* copy = (Board*)calloc(1, sizeof(Board*));
    int singleValue;
    struct Action* newAction =(struct Action*)calloc(1, sizeof(struct Action*));
    int row;
    int col;
    SP_BUFF_SET();

    if ((*gameBoard).mode == 0 || (*gameBoard).mode == 1) {
        ErrorPrinting(INVALIDCOMMAND, "autofill");
        return;
    }
    if(isErroneous(gameBoard)){
        /*maybe a different error*/
        ErrorPrinting(INVALIDCOMMAND, "autofill");
        return;
    }

    newAction->command=AUTOFILL;
    newAction->prev=gameBoard->actionP;
    /*need to free memory from p.next to the end of the list */
    ((gameBoard)->actionP)->next=newAction;
    gameBoard->actionP=newAction;

    copyBoard(gameBoard,copy,  m,  n);
    for ( row = 0; row < n*m; row++) {
        for (col = 0; col < n*m; col++) {
            singleValue=singleValidValue(copy, row, col,  m,  n);
            if((*gameBoard).board[row][col].value==0 && singleValue!=-1){
                set(row,col, singleValue, gameBoard,m,n);

            }
        }
    }

}


/*
 * checks if the value in row,col is a valid value for our game board
 */
int valid(Board *b, int row, int col) {
    int m = b->m;
    int n = b->n;
    Cell **board = (*b).board;
    int i=0,j=0,startRow=0,startCol=0;

    int value = board[row][col].value;
    /*
     * makes sure that the value doesn't exists in row
     */
    for (i = 0; i < m*n; i++) {
        if (i != col && board[row][i].value == value) {
            return 0;
        }
    }

    /*
     * makes sure that the value doesn't exists in col
     */
    for (j = 0; j < m*n; j++) {
        if (j != row && board[j][col].value == value) {
            return 0;
        }
    }

    startRow = row - row % m;
    startCol = col - col % n;
    /*
     * makes sure that the value doesn't exists in square
     */
    for ( i = startRow; i < startRow + m; i++) {
        for ( j = startCol; j < startCol + n; j++) {
            if (i != row && j != col && board[i][j].value == value) {
                return 0;
            }
        }
    }

    return 1;

}
/*
    * if there ia a single valid value , return the value. else return -1.
    * the value of the cell is 0 before and after
    */
int singleValidValue(Board *b, int row, int col, int m, int n){
    int i;
    int sum=0;
    int value;
    for (i=1;i<n*m+1; i++){
        set(row, col,i, b,m,n);
        if(valid(b, row,col)){
            if(sum==0){
                sum+=1;
                value=i;
            }
            else{
                return -1;
            }
        }
    }
    set(row, col,0, b,m,n);
    return value;


}
/*
 * maybe need to check that the boards are of the same size?
 */
void copyBoard(Board *gameBoard, Board* copy, int m, int n) {
    int i=0;
    int j =0;
    for ( i = 0; i < m*n; i++) {
        for ( j = 0; j < m*n; j++) {
            (*copy).board[i][j].value = (*gameBoard).board[i][j].value;
        }
    }

}

int notValidIndexforBoard(Board *b,int x, int y){
    int m = b->m;
    int n = b->n;
    if(x<0 || y<0 || x>=m*n || y>=m*n){
        return 1;
    }
    return 0;
}
int hint(Board *b,int x,int y){
    int result;
    /*
     * should we split it and print different error messages for every option?
     */
    if(b->mode!=2 ||  notValidIndexforBoard(b,x,y) || isErroneous(b)
    || b->board[x][y].fixed ||b->board[x][y].value!=0  ){
        ErrorPrinting(INVALIDCOMMAND,"hint");
        return 0;
    }
    result = solveUsingILP(b);
    if( result== -1){
        /*
         * print an error
         * return
         */
    }
    return result;
}



/*
 * getters and setters
 */
int getValue(Board *b, int row, int col) {
    return ((*b).board)[row][col].value;
}
void setValue(Board *b, int row, int col, int val) {
    (*b).board[row][col].value = val;
}

int getNumOfOptions(Board *b, int row, int col) {
    return (*b).board[row][col].numOfOptions;
}
void addOptions(Board *b, int row, int col, int val) {
    int numOfop = (*b).board[row][col].numOfOptions;
    (*b).board[row][col].options[numOfop] = val;
}
void setNumOfOptions(Board *b, int row, int col, int num) {
    (*b).board[row][col].numOfOptions = num;
}

int getOptionInIndex(Board *b, int row, int col, int index) {
    return (*b).board[row][col].options[index];
}

int isFixed(Board *b, int row, int col) {
    return (*b).board[row][col].fixed;
}
void setFixed(Board* b,int row,int col, int i) {
    (*b).board[row][col].fixed = i;
}


int isMarked(Board *b, int row, int col) {
    return (*b).board[row][col].marked;
}
void setMarked(Board* b,int row,int col, int i) {
    (*b).board[row][col].marked = i;

}

