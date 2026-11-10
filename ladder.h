#ifndef LADDER_H
#define LADDER_H
#include <iostream>
#include "entity.h"

// Ladder class inheriting from entity, representing the final win destination
class ladder: public entity{
public:
    ladder(char, short, int, int, int, int);
    void spawnLadder();
    int win_game(int, int);
    ~ladder();
};

#endif