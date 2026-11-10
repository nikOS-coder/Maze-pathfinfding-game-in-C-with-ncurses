#include <iostream>
#include <ncurses.h>
#include "key.h"

using namespace std;

key::key(char ch, short c, int tx, int ty, int w, int l):entity(ch, c, tx, ty, w, l){}

void key::printKey(){
    draw();
}

// Check if character coordinates match key coordinates to pick it up
int key::pickupKey(int check, int xc, int yc){
    if(check && this->x == xc && this->y == yc){
        key_is_picked = 0;
        return 1;
    }
    return 0;    
}

int key::getKEY() const{
    return key_is_picked;
}

key::~key(){}