#include <stdio.h>
#include "Game.h"
#include <stdlib.h>
#include "SPBufferset.h"
#include "MainAux.h"
#include "Game.h"

int main() {
    int m=3;
    int n=3;
    Board* b=(Board*) calloc(1, sizeof(Board));
    SP_BUFF_SET();
    if (callocFailed("main", b)) {
        free(b);
        exit(0);
    }
    b->m=3;
    b->n =3;
    if ((!intializeEmptyBoard(b))) {
        free(b);
        exit(0);
    }
    b->mode = 1;
/*
    b->board[0][1].value=6;
    b->board[0][1].fixed=0;
    b->board[3][1].value=6;
    b->board[2][1].value=8;
    b->board[6][6].value=7;
    b->board[6][6].marked=1;
    b->board[7][5].value=1;
    b->board[5][5].value=9;
    b->board[5][5].marked=9;
    b->board[0][7].value=1;
    b->board[0][7].fixed=1;
    b->board[4][8].value=4;
    b->board[3][5].value=2;
    b->board[3][5].marked=1;
    print_board(b,  m,  n);
    save(b,m,n,"C:\\Users\\Rubit\\Desktop\\henlo.txt");

*/

    edit(b,"C:\\Users\\Rubit\\Desktop\\henlo.txt");
    print_board(b,  m,  n);

    return 0;

}
