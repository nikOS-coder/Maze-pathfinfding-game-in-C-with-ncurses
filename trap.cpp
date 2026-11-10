#include <iostream>
#include <ncurses.h>
#include "trap.h"

using namespace std;

trap::trap(char ch, short c, int tx, int ty, int w, int l):entity(ch, c, tx, ty, w, l){}

void trap::spawnTrap(){
    draw();
}

// Trigger trap if player steps on its coordinates
int trap::triggered_trap(int check, int xc, int yc){
    if(check && this->x == xc && this->y == yc){
        check_trigger = 0;
        return 1;
    }
    return 0;
}

// Check deactivation condition if character with key reaches the trap
int trap::diactivate_trap(int check, int xc, int yc){
    if(!check && this->x == xc && this->y == yc){
        return 1;
    }
    return 0;
}

int trap::getTrigger() const{
    return check_trigger;
}

trap::~trap(){}