#include "utils.h"
#include <ncurses.h>

// Inspect screen character at specific coordinates using ncurses mvinch
char getting_char(int x, int y){
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if(x < 0 || x >= max_x || y < 0 || y >= max_y){
        return '*';
    }

    chtype ch = mvinch(y ,x);
    return ch & A_CHARTEXT;
}