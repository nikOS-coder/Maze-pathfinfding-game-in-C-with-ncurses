#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>

using namespace std;

// Base class representing any interactive entity in the game world
class entity{
protected:
    int x, y;         // Coordinate positions on the screen
    short color;      // Ncurses color pair ID
    char letter;      // Character representation rendered on screen
public:
    entity(char, short, int, int, int, int);
    int check_spawn(int, int, int, int);
    void draw();

    int getX() const;
    int getY() const;
    char getLetter() const;
    void setX(int);
    void setY(int);
    void setletter(char);
    virtual ~entity();
};

#endif