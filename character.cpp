#include <iostream>
#include <ncurses.h>
#include <stack>
#include <set>
#include "character.h"
#include "utils.h"

using namespace std;

character::character(char ch, short c, int tx, int ty, int w, int l): entity(ch, c, tx, ty, w, l){}

// Core automated movement logic utilizing Depth-First Search (DFS) and backtracking
bool character::player_move(int topx, int topy, int width, int length, bool &need_for_key, int &termina){

    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {-1, 1, 0, 0};
    int nx, ny, px, py, i;
    bool check;

    visited.insert({x , y});

    // If key requirement flag is active, push current position to backtrack history
    if(need_for_key == true){
        backtrack.push({x, y});
    }

    // If key is obtained, execute return path using the backtrack stack
    if(has_key == 1 && need_for_key == true){
        while(!backtrack.empty()){
            px = backtrack.top().first;
            py = backtrack.top().second;
            backtrack.pop();
            move(y, x);
            addch(' ');
            x = px;
            y = py;
            draw();
            termina++;
            mvprintw(1, 0, "current termina : %d", termina);
            if(termina == 1000){
                break;
            }
            napms(120);
            refresh();
        }
        return true;
    }
 
    // Explore adjacent directions (up, down, right, left)
    for(i = 0; i < 4; i++){
        nx = x + dx[i];
        ny = y + dy[i];

        check = false;
        if(visited.count({nx, ny}) == 0){
            check = true;
        }

        // Move forward if cell is walkable and unvisited
        if(((getting_char(nx, ny) == ' ' || getting_char(nx, ny) == 'T' || getting_char(nx, ny) == 'K' || getting_char(nx, ny) == 'L' || getting_char(nx, ny) == 'G' || getting_char(nx, ny) == 'S') && check)){
            paths.push({x, y});
            move(y, x);
            addch(' ');
            x = nx;
            y = ny;
            draw();
            napms(60);
            refresh();
            return true;
        }

        // Handle collision with cage 'C' when key is held
        else if(getting_char(nx, ny) == 'C' && has_key == 1){
            move(y, x);
            addch(' ');
            x = nx;
            y = ny;
            draw();
            return true;
        }

        // Encounter cage without key - flag need for key and store position
        else if(getting_char(nx, ny) == 'C' && has_key == 0){
            need_for_key = true;
            backtrack.push({nx, ny});
        }
    }

    // Backtrack using the paths stack if no unvisited adjacent cells remain
    if(!paths.empty()){
        px = paths.top().first;
        py = paths.top().second;
        paths.pop();
        move(y, x);
        addch(' ');
        x = px;
        y = py;
        draw();
        napms(60);
        refresh();
        return true;
    }

    return false;
}

void character::keycheck(int a){
    if(a == 1){
        has_key = 1;
    }
}

int character::getKey() const{
    return has_key;
}

// Render the character enclosed in a trap cage 'C'
void character::cage(){
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(y, x, "%c", 'C');
    attroff(COLOR_PAIR(1));
}

void character::is_trapped(){
    trapped = 1;
}

void character::is_untrapped(){
    draw();
    trapped = 0;
}

int character::getTrap() const{
    return trapped;
}

// Navigate character towards ladder coordinates upon winning
void character::player_to_ladder(int xl, int yl){
    move(y, x);
    addch(' ');
    if(x == xl){}
    else{
        x > xl ? x-- : x++;
    }
    if(y == yl){}
    else{
        y > yl ? y-- : y++;
    }
    draw();
    napms(150);
}

character::~character(){}