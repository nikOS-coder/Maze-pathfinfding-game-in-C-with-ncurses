#ifndef TRAP_H
#define TRAP_H
#include <iostream>
#include "entity.h"

using namespace std;

// Trap class inheriting from entity, managing activation and deactivation states
class trap: public entity{
private:
    int check_trigger = 1; // 1 = active trap, 0 = triggered/disarmed
public:
    trap(char, short, int, int, int, int);
    void spawnTrap();
    int triggered_trap(int, int, int);
    int diactivate_trap(int, int, int);
    int getTrigger() const;
    ~trap();
};

#endif