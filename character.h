#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>
#include <stack>
#include <set>
#include "entity.h"

using namespace std;

// Character class inheriting from entity, managing automated pathfinding, traversal stacks, and traps
class character: public entity{
private:
    int has_key = 0;
    int trapped = 0;
    set<pair<int,int>> visited;     // Tracks visited coordinates to avoid loops
    stack<pair<int, int>> paths;    // Stack keeping track of forward path history for DFS traversal
    stack<pair<int, int>> backtrack;// Stack used for backtracking when key is acquired
public:
    character(char, short, int, int, int, int);
    bool player_move(int, int, int, int, bool &, int&);
    void keycheck(int);
    int getKey() const;
    void cage();
    void is_trapped();
    void is_untrapped();
    int getTrap() const;
    void player_to_ladder(int, int);
    ~character();
};

#endif