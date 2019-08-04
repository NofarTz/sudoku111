
#include "gameTester.h"
#include "Game.h"
#include <stdlib.h>
#include "MainAux.h"
int gameTest(){
    Board* gameBoard=(Board*) calloc(1, sizeof(Board));
    if (callocFailed("main", gameBoard)) {
        free(gameBoard);
        exit(0);
    }
    return 1;
}
