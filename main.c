#include <stdio.h>
#include "Game.h"
#include <stdlib.h>
#include "SPBufferset.h"
#include "MainAux.h"
#include "Game.h"

int main() {
    int i;
    Board *b = (Board *) calloc(1, sizeof(Board));
    SP_BUFF_SET();
    if (callocFailed("main", b)) {
        free(b);
        exit(0);
    }
    b->m = 3;
    b->n = 3;
    if ((!intializeEmptyBoard(b))) {
        free(b);
        exit(0);
    }
    b->mode = 2;
    /*b->actionStart = (struct Action *) calloc(1, sizeof(struct Action *));
    b->actionStart->next=0;
    b->actionP=b->actionStart;*/
   /* b->board[0][1].value = 6;
    b->board[0][1].fixed = 1;
    b->board[2][1].value = 8;
    b->board[6][6].value = 7;
    b->board[7][5].value = 1;
    b->board[5][5].value = 9;
    b->board[0][7].value = 1;
    b->board[0][7].fixed = 1;
    b->board[4][8].value = 4;
    b->board[4][7].value = 3;
    b->board[3][5].value = 2;
    set(0,5,3);
    */

    b->board[0][0].value = 1;
    b->board[0][1].value = 2;
    b->board[0][2].value = 3;
    b->board[0][3].value = 4;
    b->board[0][4].value = 5;
    b->board[0][5].value = 6;
    b->board[0][6].value = 7;
    b->board[0][7].value = 8;
    print_board(b);
    i=markBoard(b);
    printf("%d\n",i);
    autofill(b, 3, 3);


    free(b);
return 0;

}
