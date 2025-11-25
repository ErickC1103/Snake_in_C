#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//functions used for moving the cursor
void moveRight(int  * cx,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    (*cx)++;//right
    }
}
void moveLeft(int  * cx,int dist)
{
   for(int i = 0; i < dist; ++i)//move right
   {
    (*cx)--;//left
   }
}
void moveDown(int * cy,int dist)
{
    for(int i = 0; i < dist;++i)
    {
        (*cy)++;//down
    }
}
void moveUp(int * cy,int dist)
{
    for(int i = 0; i < dist;++i)
    {
        (*cy)--;//up
    }
}
void moveDownLeft(int * cx,int * cy,int dist)
{
    for(int i = 0; i < dist;++i)
    {
        (*cx)--;//left
        (*cy)++;//down
    }
}
void moveUpLeft(int * cx,int * cy,int dist)
{
    for(int i = 0; i < dist;++i)
    {
        (*cx)--;//left
        (*cy)--;//Up
    }
}


//functions for drawing with the cursor
void goLeft(int * cx,int cy,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cy,*cx," ");
    (*cx)--;
    refresh();
   }
}
void goRight(int  * cx,int cy,int dist)
{
   for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cy,*cx," ");
    (*cx)++;
    refresh();
    }
}
void goUp(int cx,int * cy,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(*cy,cx," ");
    (*cy)--;
    refresh();
   }
}
void goDown(int * cx,int * cy,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(*cy,*cx," ");
    (*cy)++;
    refresh();
    }
}
void goDiaDown(int * cx,int * cy,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(*cy,*cx," ");
    (*cx)++;
    (*cy)++;
    refresh();
    }
}
void goDiaUp(int* cx,int * cy,int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(*cy,*cx," ");
    (*cx)++;
    (*cy)--;
    refresh();
    }
}
void drawA(int x, int y)
{
   goDiaUp(&x,&y,4);
   goDiaDown(&x,&y,5);
   moveUpLeft(&x,&y,3);
   goLeft(&x,y,4);
  
   
}

void drawS(int x, int y)
{
    
   goRight(&x,y,3);  
   goUp(x,&y,2);
   goLeft(&x,y,3);
   goUp(x,&y,2);
   goRight(&x,y,4);
   
  
    
}
void drawK(int x, int y)
{
    goUp(x,&y,5);
    moveDown(&y,3);
    moveRight(&x,1);
    goDiaUp(&x,&y,3);
    moveDownLeft(&x,&y,3);
    goDiaDown(&x,&y,3);
    
   

}
void drawE(int x,int y)
{
    goUp(x,&y,4);
    goRight(&x,y,5);
    moveLeft(&x,5);
    moveDown(&y,2);
    goRight(&x,y,3);
    moveLeft(&x,3);
    moveDown(&y,2);
    goRight(&x,y,5);

    
}
void drawN(int x,int y)
{
    goUp(x,&y,4);
    goDiaDown(&x,&y,4);
    goUp(x,&y,5);
  
    
}
void createSnakeTitle(int x,int y)
{
    int spacing = 5;
    int length;
   
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // White text on blue background
    attron(COLOR_PAIR(2));
    drawS(x,y);
    x += 7;
    drawN(x,y);
    x += 8;
    drawA(x,y);
    x += 12;
    drawK(x,y);
    x += 6;
    drawE(x,y);
    attroff(COLOR_PAIR(2));
    // x+=spacing;
    // drawN(x,y);
    // x+=spacing;
    // drawA(x,y);
    // x+=spacing;
    // drawK(x,y);

    
    
}
int main()
{  
    int x =10;
    int y = 5;
    initscr();
    start_color();
    noecho();
    curs_set(FALSE);
    createSnakeTitle(x,y);
    
    
    int key;
    while((key = getchar()) != 'q')
    {

    }
    
    endwin();   
    return 0;
}