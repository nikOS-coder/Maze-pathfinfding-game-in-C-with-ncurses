#include "ladder.h"
#include <iostream>
#include <ncurses.h>

using namespace std;

ladder::ladder(char ch, short c, int tx, int ty, int w, int l): entity(ch, c, tx, ty, w, l){}

void ladder::spawnLadder(){
    draw();
}

// Check if a character has reached the ladder coordinates
int ladder::win_game(int xc, int yc){
    if(x == xc && y == yc){
        return 1;
    }
    return 0;
}

ladder::~ladder(){}