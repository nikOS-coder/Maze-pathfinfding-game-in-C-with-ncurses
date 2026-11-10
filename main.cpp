#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <set>
#include "character.h"
#include "key.h"
#include "trap.h"
#include "ladder.h"
#include "utils.h"

using namespace std;

void maze_width_length(ifstream&, int&, int &);
char **print_maze(ifstream&, int, int, char**, int & , int&, int&, int&);
int check_position_characters(int, int, int, int);
void check_trap(trap &, character &);
void key_and_trap(character &, int &);
void save_from_trap(trap &, character &, character &);
void delete_maze(int, int, int, int);
void win_game(character &, character &, int &, int &);
int check_overlap(character &, character &, trap &, trap &, key &, ladder &);
void termina_check(int, int &);

int main(int argc, char **argv){
    // Validate command-line arguments (requires maze file parameter)
    try{
        if(argc != 2){
            throw "type <executable_programm> <file_with_maze_name>";
        }
    }
    catch(const char *str){
        cerr << str << endl;
        exit(-1);
    }

    ifstream mazefile(argv[1]);
    try{
        if(!mazefile.is_open()){
            throw "File was not found!";
        }
    }
    catch(const char *str){
        cerr << str << endl;
        exit(-1);
    }

    // Initialize random seed and ncurses terminal settings
    srand(time(NULL));
    initscr();
    start_color();
    noecho();
    cbreak();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    int i;

    curs_set(0);
    string start = "Press any button to start ...";
    for(i = 0; i < start.size(); i++){
        mvaddch(0, i, start[i]);
        refresh();
        napms(50);
    }
    bool active = true;
    nodelay(stdscr, true);
    while(true){
        if(active){
            mvprintw(0 , 26, "...");
        }
        else{
            mvprintw(0 , 26, "   ");
        }
        active = !active;
        napms(500);
        refresh();
        int ch = getch();
        if(ch != ERR){
            break;
        }
    }

    nodelay(stdscr, false);
    clear();
    refresh();
    
    // Determine maze file dimensions (width and length)
    int length = 0, width = 0;
    maze_width_length(mazefile, width, length);
    char **maze = new char*[width];
    for(i = 0; i < width; i++){
        maze[i] = new char[length];
    }
    
    // Calculate centered placement coordinates on screen
    int max_height, max_width, lx, ly;
    getmaxyx(stdscr, max_height, max_width);
    int top_leftx = (max_width - length) / 2;
    int top_lefty = (max_height - width) / 2;
    maze = print_maze(mazefile, top_lefty, top_leftx, maze, width, length, lx, ly);

    // Instantiate game entities (Characters G & S, Key K, Traps T1 & T2, Ladder L)
    character G('G', 3, top_leftx, top_lefty, width, length);
    character S('S', 2, top_leftx, top_lefty, width, length);
    key K('K', 4, top_leftx, top_lefty, width, length);
    trap T1('T', 5, top_leftx, top_lefty, width, length);
    trap T2('T', 5, top_leftx, top_lefty, width, length);
    ladder L('L', 5, top_leftx, top_lefty, width, length);

    L.setX(lx);
    L.setY(ly);
    
    // Randomize entity spawn locations until no overlap occurs
    do{
        while(check_position_characters(G.getX(), G.getY(), S.getX(), S.getY()) == 0 || G.check_spawn(top_leftx, top_lefty, width, length) == 0 || S.check_spawn(top_leftx, top_lefty, width, length) == 0){
            G.setX(rand() % length + top_leftx);
            G.setY(rand() % width + top_lefty);
            S.setX(rand() % length + top_leftx);
            S.setY(rand() % width + top_lefty);    
        }

        while(!K.check_spawn(top_leftx, top_lefty, width, length)){
            K.setX(rand() % length + top_leftx);
            K.setY(rand() % width + top_lefty);
        }

        while(!T1.check_spawn(top_leftx, top_lefty, width, length)){
            T1.setX(rand() % length + top_leftx);
            T1.setY(rand() % width + top_lefty);
        }

        while(!T2.check_spawn(top_leftx, top_lefty, width, length)){
            T2.setX(rand() % length + top_leftx);
            T2.setY(rand() % width + top_lefty);
        }
    }while(check_overlap(G, S, T1, T2, K, L));

    bool Gkey = false, Skey = false;

    T1.spawnTrap();
    T2.spawnTrap();
    K.printKey();
    int check = 1, termina = 0, win = 0;
    
    // Main game execution loop
    do{
        if(G.getTrap() == 0){ 
            if(G.player_move(top_leftx, top_lefty, width, length, Gkey, termina)){
                termina++;
            }
            G.keycheck(K.pickupKey(K.getKEY(), G.getX(), G.getY()));
            check_trap(T1, G);
            check_trap(T2, G);
            win_game(G, S, check, win);
            mvprintw(1, 0, "current termina : %d", termina);
            termina_check(termina, check);
            napms(60);
        }
        if(S.getTrap() == 0){
            if(S.player_move(top_leftx, top_lefty, width, length, Skey, termina)){
                termina++;
            }
            S.keycheck(K.pickupKey(K.getKEY(), S.getX(), S.getY()));
            check_trap(T1, S);
            check_trap(T2, S);
            win_game(G, S, check, win);
            mvprintw(1, 0, "current termina : %d", termina);
            termina_check(termina, check);
            napms(60);
        }

        if(G.getTrap() == 1){
            G.cage();
        }
        if(S.getTrap() == 1){
            S.cage();
        }

        save_from_trap(T1, G, S);
        save_from_trap(T1, S, G);
        save_from_trap(T2, G, S);
        save_from_trap(T2, S, G);
            
        if(G.getTrap() == 1 && S.getTrap() == 1){
            mvprintw(0, 0, "GAME OVER!");
            check = 0;
        }

        key_and_trap(G, check);
        key_and_trap(S ,check);

    }while(check);
    
    // Handle win condition and movement toward ladder
    if(win == 1){
        delete_maze(top_lefty, top_leftx, width, length);
        L.spawnLadder();
        while(!L.win_game(G.getX(), G.getY()) || !L.win_game(S.getX(), S.getY())){
            G.player_to_ladder(L.getX(), L.getY());
            S.player_to_ladder(L.getX(), L.getY());
        }
    }

    // End-game exit screen animation and prompt
    flushinp();
    string end = "Press any button to end ...";
    for(i = 0; i < end.size(); i++){
        mvaddch(max_height - 1, max_width - 27 + i, end[i]);
        refresh();
        napms(50);
    }
    active = true;
    nodelay(stdscr, true);
    while(true){
        if(active){
            mvprintw(max_height - 1, max_width - 3, "...");
        }
        else{
            mvprintw(max_height - 1, max_width - 3, "   ");
        }
        active = !active;
        napms(500);
        refresh();
        int ch = getch();
        if(ch != ERR){
            break;
        }
    }
    nodelay(stdscr, false);

    mazefile.close();
    flushinp();
    endwin();
    
    // Clean up dynamically allocated memory for maze grid
    for(i = 0; i < width; i++){
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}

// Helper function to calculate maze file rows and columns
void maze_width_length(ifstream &file, int &width, int &length){
    file.seekg(0, ios::beg);
    while(file.get() != '\n'){
        length++;
    }
    file.seekg(0, ios::beg);
    
    string line;
    while(!file.eof()){
        if(getline(file, line)){
            width++;
        }
    }
}

// Render maze characters to the terminal screen
char **print_maze(ifstream &file, int height, int width, char **pmaze, int &maze_width, int &maze_length, int &lx, int &ly){
    file.seekg(0, ios::beg);
    string line;
    int i, j;
    for(i = 0; i < maze_width; i++){
        getline(file, line);
        for(j = 0; j < maze_length; j++){
            if(line[j] != '\n'){
                if(line[j] == 'L'){
                    line[j] = ' ';
                    lx = width + j;
                    ly = height + i;
                }
                pmaze[i][j] = line[j];
                mvprintw(height+i, width+j, "%c", pmaze[i][j]);
            }
            else{
                j--;
            }
        }   
    }
    refresh();
    return pmaze;
}

int check_position_characters(int xg, int yg, int xs, int ys){
    if(abs(xg - xs) < 7 || abs(yg - ys) < 7){
        return 0;
    }
    return 1;
}

void check_trap(trap &T, character &C){
    if(T.triggered_trap(T.getTrigger(), C.getX(), C.getY())){
        C.is_trapped();
        attron(COLOR_PAIR(7));
        mvprintw(2, 0, "A player was trapped!");
        attroff(COLOR_PAIR(7));
    }
}

void save_from_trap(trap &T, character &Cr, character &Cc){
    if(Cr.getKey() == 1 && T.diactivate_trap(T.getTrigger(), Cr.getX(), Cr.getY()) && !Cr.getTrap()){
        Cc.is_untrapped();
    }
}

void delete_maze(int height, int width, int maze_width, int maze_length){
    int i, j;
    for(i=0; i<maze_width; i++){
        for(j=0; j<maze_length; j++){
            if(getting_char(width + j, height + i) == '*' || getting_char(width + j, height + i) == 'T' || getting_char(width + j, height + i) == 'K'){
                mvprintw(height+i, width+j, " ");
                refresh();
                napms(3);
            }
        }
    }
}

void key_and_trap(character &C, int &check){
    if(C.getKey() && C.getTrap()){
        mvprintw(0, 0, "GAME OVER!");
        check = 0;
    }
}

void win_game(character &CG, character &CS, int &check, int &win){
    if(CG.getX() == CS.getX() && CG.getY() == CS.getY()){
        attron(COLOR_PAIR(5));
        mvprintw(0, 0, "YOU WON!");
        attroff(COLOR_PAIR(5));
        win = 1;
        check = 0;
    }
}

int check_overlap(character &CG, character &CS, trap &T1, trap &T2, key &K, ladder &L){
    if((CG.getX() == T1.getX() && CG.getY() == T1.getY()) || (CG.getX() == T2.getX() && CG.getY() == T2.getY()) ||
    (CG.getX() == K.getX() && CG.getY() == K.getY()) || (CG.getX() == L.getX() && CG.getY() == L.getY()) ||
    (CS.getX() == T1.getX() && CS.getY() == T1.getY()) || (CS.getX() == T2.getX() && CS.getY() == T2.getY()) ||
    (CS.getX() == K.getX() && CS.getY() == K.getY()) || (CS.getX() == L.getX() && CS.getY() == L.getY()) ||
    (T1.getX() == T2.getX() && T1.getY() == T2.getY()) || (T1.getX() == K.getX() && T1.getY() == K.getY()) ||
    (T1.getX() == L.getX() && T1.getY() == L.getY()) || (T2.getX() == K.getX() && T2.getY() == K.getY()) ||
    (T2.getX() == L.getX() && T2.getY() == L.getY()) || (K.getX() == L.getX() && K.getY() == L.getY())){
        return 1;
    }
    return 0;
}

void termina_check(int a, int &check){
    if(a == 1000){
        check = 0;
        mvprintw(0, 0, "GAME OVER!");
    }
}