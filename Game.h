
#ifndef SUDOKU2_0_GAME_H
#define SUDOKU2_0_GAME_H
enum Command {
    SET, HINT, VALIDATE, RESTART, EXIT,AUTOFILL
};

typedef struct {
    int value;
    int fixed;
    int numOfOptions;
    int *options;
    int marked;
} Cell;
struct Action  {
    enum Command command;
    int *arg;
    struct Action *next;
    struct Action *prev;

};



typedef struct {
    Cell **board;
    int m;
    int n;
    int isSolved;
    int markErrors;
    /*
     * mode == 0 - init mode
     * mode == 1 - edit mode
     * mode == 2 - solve mode
     */
    int mode;
    struct Action actions;
    struct Action *actionP;
}Board;

void copyBoard(Board *gameBoard, Board* copy, int m, int n);
int valid(Board *b, int row, int col);
int singleValidValue(Board *b, int row, int col, int m, int n);
void autofill(Board *gameBoard, int m, int n);
int markErrors(Board *b, int row, int col);
int boardIsFilled(Board *gameBoard);
int markBoard(Board *b);
void unMarkBoard(Board *b);
void mark_errors(Board *b, int x);
int save(Board *b,  char *path);
int intializeEmptyBoard(Board* b);
void set(int x, int y, int z, Board *gameBoard,int m,int n);
int isErroneous(Board *b);
int edit(Board *b, char *path);
void tmp(char *path);

/*
 * getters and setters
 */

/*
 * gets value in position row,col
 */
int getValue(Board *b, int row, int col);
/*
 * sets the value in position row,col to val
 */
void setValue(Board *b, int row, int col,int val);
/*
 * gets the number of possible options for values for position row,col
 */
int getNumOfOptions(Board *b, int row, int col);
/*
 * adds a new option, val, to the options for position row,col
 */
void addOptions(Board *b, int row, int col, int val);
/*
 * sets the number of options for position row,col to num
 */
void setNumOfOptions(Board *b, int row, int col, int num);
/*
 * gets the n-th option for position row,col (n = index)
 */
int getOptionInIndex(Board *b, int row, int col, int index);
/*
 * returns 1 is position row,col is fixes. ow 0.
 */
int isFixed(Board *b, int row, int col);
/*
 * if i =1 - sets position row,col to be fixed. if i=0 - sets position row,col to be not fixed
 */
void setFixed(Board* b,int row,int col,int i);

int isMarked(Board *b, int row, int col);

void setMarked(Board* b,int row,int col, int i);


#endif
