#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INITIAL_LENGTH 3//would define the initial length of the snake
#define max_snake_length 1000
#define START_TIME_DELAY 100000
#define FOOD_DECREMENT 500
struct Point// this would be used to refer to the snake body/head and the food
{
    int x , y;   
};
//global variables
int snake_len;
struct Point snake[max_snake_length];
struct Point food;
int score = 0;
int high_score = 0;
int field_top,field_bottom,field_left,field_right;
int delay = START_TIME_DELAY;
//the game would have to interact with the snake, the border,the direction etc
//there fore they would become global in order to not pass so many parameters

void moveSnake(char * direction)
{
    
}

void drawBorder()//makes the border
{//start of create Border
    init_pair(1,COLOR_BLUE,COLOR_BLUE);//color for the background
    
    
    WINDOW * win = newwin(field_bottom,field_right,0,0);//create a window
    refresh();//refresh

    wattron(win, COLOR_PAIR(1));//applying the background color 
    box(win,' ',' ');//make a box 
    wattroff(win, COLOR_PAIR(1)); //end background clolor

    wrefresh(win);//window refresh
}
void drawScore()//would display the score on the bottom of the screen
{
  int score_pos_y = field_bottom + 1;
  int score_pos_x = field_left;
  mvprintw(score_pos_y,score_pos_x,"Score = %d",score);
  mvprintw(score_pos_y + 1,score_pos_x,"High score = %d",high_score);
  refresh(); 
}
void drawSnake()//draws the snake figure
{//start of drawSnake
    init_pair(2,COLOR_GREEN,-1);//color for the head
    init_color(11,0,400,0);//initializing color for the body
    init_pair(3,11,-1);//color for the body
    if(snake_len > 0)//for the snake head
    {
        attron(COLOR_PAIR(2)); 
        mvprintw(snake[0].y,snake[0].x,"o");
        attroff(COLOR_PAIR(2)); 
    }
    for(int i = 1; i < snake_len; ++i)//for the snake body
    {
        attron(COLOR_PAIR(3));
       mvprintw(snake[i].y,snake[i].x,"o");
       attroff(COLOR_PAIR(3));
    }
    refresh();
}
void init_game()//initializes content in game
{
    clear();
    refresh();
    
    int max_y, max_x;
    max_y = LINES;
    max_x = COLS;
    snake_len = INITIAL_LENGTH;//inittialize snake length
    //initializing sides of field
    field_top = 1;//top field
    field_left = 1;//left field
    field_bottom = max_y-4;
    field_right = max_x;
    int start_x = max_x / 2;
    int start_y = max_y / 2;
    for(int i = 0; i < snake_len;++i)
    {
        snake[i].x = start_x - i;
        snake[i].y = start_y;
    }
    score = 0;
    // createBorder();
    // drawSnake();
    

}
int main()
{//start of main
   //int speed = 10;
    
    initscr();//intitialize screen
    noecho();//applies no echo
    curs_set(0);//removes the cursor
    start_color();//allows color
    use_default_colors();//allows transparent backgrounds
    keypad(stdscr,TRUE);

    
    int key;            /* wait for user input */
    init_game();

    do
    {
      drawBorder();
      drawSnake();
      drawScore();
    }
    while((key = getch()) != 'q');
    
    
    endwin();   
    return 0;
}