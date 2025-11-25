#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ENTER 13

void createBorder()
{
   int max_y, max_x,x,y;
    max_y = LINES;
    max_x = COLS;
    x = 0;
    y = 0;
    WINDOW * win = newwin(max_y,max_x,x,y);
    keypad(win,TRUE);
    refresh();
    
    box(win,'.','.');
    wrefresh(win);
}

int main()
{
    //int speed = 10;
    
    initscr();
    noecho();
    
    int key;            /* wait for user input */
    do
    {
      createBorder();
    }
    while((key = getchar()) != ENTER);
    
    
    endwin();   
    return 0;
}