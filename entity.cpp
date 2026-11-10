#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ncurses.h>
#include "entity.h"
#include "utils.h"

using namespace std;

// Constructor to initialize entity properties and randomize valid coordinates within maze bounds
entity::entity(char ch, short id_pair, int topleftx, int toplefty, int maze_width, int maze_length){
    x = rand() % maze_length + topleftx;
    y = rand() % maze_width + toplefty;
    letter = ch;
    color = id_pair;
}

// Validate if the entity has spawned on a walkable path character (' ')
int entity::check_spawn(int topleftx, int toplefty, int maze_width, int maze_length){
    if(x > topleftx && x < (topleftx + maze_length) && y > toplefty && y < (toplefty + maze_width)){
        if(getting_char(x, y) == ' '){
            return 1;
        }
    }
    return 0;
}

// Render the entity on the ncurses terminal window using its specific color pair
void entity::draw(){
    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%c", letter);
    attroff(COLOR_PAIR(color));
    refresh();
}

int entity::getX() const{
    return x;
}

int entity::getY() const{
    return y;
}

char entity::getLetter() const{
    return letter;
}

void entity::setX(int newx){
    x = newx;
}

void entity::setY(int newy){
    y = newy;
}

void entity::setletter(char newletter){
    letter = newletter;
}

entity::~entity(){}