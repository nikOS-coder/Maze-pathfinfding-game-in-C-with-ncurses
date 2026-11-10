#ifndef KEY_H
#define KEY_H
#include <iostream>
#include "entity.h"

using namespace std;

// Key class inheriting from entity, handling key pickup mechanics
class key: public entity{
private:
    int key_is_picked = 1; // 1 means active/uncollected, 0 means picked up
public:
    key(char, short, int, int, int, int);
    void printKey();
    int pickupKey(int, int, int);
    int getKEY() const;
    ~key();
};

#endif