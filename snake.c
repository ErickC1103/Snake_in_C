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
struct Point snake[max_snake_length];
int snake_len;
struct Point food;
int score = 0;
int high_score = 0;
int top_score = 0;
int field_top,field_bottom,field_left,field_right;
int delay = START_TIME_DELAY;
int cursor_x = 10;
int cursor_y = 5;

//the game would have to interact with the snake, the border,the direction etc
//there fore they would become global in order to not pass so many parameters
void draw();
void init_game();
void moveSnake();
int collides_with_wall();
int collides_with_snake(int,int);
int collides_with_self();
void place_food();
void createSnakeTitle(int,int);
void throw_title_screen();

enum DIRECT
{
    Up,
    Down,
    Left,
    Right,
    Stop
}direction;

void moveSnake()
{
    if(direction == Stop)
    {
        return;
    }
    for(int i = snake_len-1 ;i >= 1; --i)
    {
        snake[i] = snake[i-1];
    }
    switch(direction)
    {
        case Up:
            snake[0].y -= 1;
            break;
        case Down:
            snake[0].y += 1;
            break;
        case Left:
            snake[0].x -= 1;
            break;
        case Right:
            snake[0].x += 1;
            break;
        default:
    }
}



void draw()
{
    clear();

   //for drawing border
    init_pair(1,COLOR_BLUE,COLOR_BLUE);//color for the head
    attron(COLOR_PAIR(1));
    for(int x = field_left; x <= field_right;++x)
    {
        mvaddch(field_top,x,' ');
        mvaddch(field_bottom,x,' ');
    }
    for(int y = field_top; y <= field_bottom; ++y)
    {
       mvaddch(y,field_left,' ');
       mvaddch(y,field_right,' '); 
    }
    attroff(COLOR_PAIR(1));
  //draw the score
   
    mvprintw(field_bottom + 1,field_left,"Score = %d  HighScore = %d Top score = %d ",score,high_score,top_score);
    mvprintw(field_bottom + 2,field_left,"Controls: arrow keys | q to quit");
   // for drawing the snake
    init_pair(2,COLOR_GREEN,-1);//color for the head
    init_color(11,0,400,0);//initializing color for the body
    init_pair(3,11,-1);//color for the body
    if(snake_len > 0)//for the snake head
    {
        attron(COLOR_PAIR(2)); 
        mvaddch(snake[0].y,snake[0].x,'o');
        attroff(COLOR_PAIR(2)); 
    }
    for(int i = 1; i < snake_len; ++i)//for the snake body
    {
        attron(COLOR_PAIR(3));
       mvaddch(snake[i].y,snake[i].x,'o');
       attroff(COLOR_PAIR(3));
    }
    //for drawing the food
    mvaddch(food.y,food.x,'.');

    refresh();
}
void place_food()
{
    int x,y;
    do{
        x = (rand() % (field_right - field_left - 1)) + field_left + 1;
        y = (rand() % (field_bottom - field_top - 1)) + field_top + 1;      
    } while(collides_with_snake(x,y));
    food.x = x;
    food.y = y;
}

int collides_with_wall()
{
    if(snake[0].x <= field_left || snake[0].x >= field_right ||
    snake[0].y <= field_top || snake[0].y >= field_bottom)
    {
        return 1;
    }
    return 0;
}



int collides_with_snake(int x,int y)
{
    for(int i = 0; i < snake_len; ++i)
    {
        if(snake[i].x == x && snake[i].y == y)
        {
            return 1;
        }
    }
    return 0;
}
int collides_with_self()
{
    for(int i = 1; i < snake_len; ++i)
    {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return 1;
        }
    }
    return 0;
}
void init_game()//initializes content in game
{
    
    
    int max_y, max_x;
    max_y = LINES;
    max_x = COLS;
    snake_len = INITIAL_LENGTH;//inittialize snake length
    top_score = COLS;
    //initializing sides of field
    field_top = 1;//top field
    field_left = 1;//left field
    field_bottom = max_y-4;//bottom field
    field_right = max_x - 1;//right field
    int start_x = max_x / 2;//starting x point for snake
    int start_y = max_y / 2;//starting y point for snake
    for(int i = 0; i < snake_len;++i)
    {
        snake[i].x = start_x - i;
        snake[i].y = start_y;
    }
    direction = Stop;
    score = 0;
    delay = START_TIME_DELAY;
    place_food();
    clear();
    draw();

}

int main()
{//start of main
   //int speed = 10;
    
    initscr();//intitialize screen
    noecho();//applies no echo
    curs_set(0);//removes the cursor
    cbreak();
    start_color();//allows color
    use_default_colors();//allows transparent backgrounds
    keypad(stdscr,TRUE);
    throw_title_screen();
    init_game();
    nodelay(stdscr,TRUE);

    
    int key;     
   
       
    

    while(1)
    {
       
        key = getch();
        if(key != ERR)
        {
            if(key == 'q' || key == 'Q') 
            {break;}
            switch(key)
           {
            case KEY_UP:
                if(direction != Down){direction = Up;}
                break;
            case KEY_DOWN:
                if(direction != Up){direction = Down;}
                break;
            case KEY_LEFT:
                if(direction != Right){direction = Left;}
                break;
            case KEY_RIGHT:
                if(direction != Left){direction = Right;}
                break;
            default:
           }
        }
        
        moveSnake();
        draw();
        
        //check for collision with wall
        if(collides_with_wall())
        {
            
            mvprintw(field_bottom + 2, field_left,"You hit a wall! Press any key to continue");
            sleep(1);
            nodelay(stdscr,FALSE);
            getch();
            nodelay(stdscr,TRUE);

            init_game();
            continue;
        }
        //checks for collision with self
        if(collides_with_self())
        {
            sleep(1);
            mvprintw(field_bottom + 2,field_left, "You ran into yourself! Press any key to continue   ");
            nodelay(stdscr,FALSE);
            getch();
            nodelay(stdscr,TRUE);
            init_game();
            continue;
        }


        //check for food 
        if(snake[0].x == food.x && snake[0].y == food.y)
        {
            snake_len++;
            score += 1;
           if(score > high_score)
            {
              high_score = score;
            }
            if(delay > FOOD_DECREMENT + 1000)
            {
             delay -= FOOD_DECREMENT;
            }
            place_food();
        }
        if(direction == Up || direction == Down)
        {
            usleep(delay+1000);
        }
        else
        {
          usleep(delay);
        }


        
    }
    
    nodelay(stdscr,FALSE);
    mvprintw(field_bottom + 2, field_left, "Thanks for playing! press any key to exit...");
    getch();
    endwin();   
    return 0;
}
void throw_title_screen()
{
    createSnakeTitle(cursor_x,cursor_y);
    mvprintw(cursor_y,cursor_x,"Press Enter to Play");
    int key;
    do
    {
       
    }
    while((key = getch()) != '\n');
}








//title screen code
void moveRight(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    cursor_x++;//right
   }
}
void moveLeft(int dist)
{
   for(int i = 0; i < dist; ++i)//move right
   {
    cursor_x--;//left
   }
}
void moveDown(int dist)
{
    for(int i = 0; i < dist;++i)
    {
        cursor_y++;//down
    }
}
void moveUp(int dist)
{
    for(int i = 0; i < dist;++i)
    {
        cursor_y--;//up
    }
}
void moveDownLeft(int dist)
{
    for(int i = 0; i < dist;++i)
    {
        cursor_x--;//left
        cursor_y++;//down
    }
}
void moveUpLeft(int dist)
{
    for(int i = 0; i < dist;++i)
    {
        cursor_x--;//left
        cursor_y--;//Up
    }
}


//functions for drawing with the cursor
void goLeft(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x--;
    refresh();
   }
}
void goRight(int dist)
{
   for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    refresh();
    }
}
void goUp(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_y--;
    refresh();
   }
}
void goDown(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_y++;
    refresh();
    }
}
void goDiaDown(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    cursor_y++;
    refresh();
    }
}
void goDiaUp(int dist)
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    cursor_y--;
    refresh();
    }
}
int drawA()
{
   goDiaUp(4);
   goDiaDown(5);
   moveUpLeft(3);
   goLeft(4);
   return 9;   
}

int drawS()
{   
   goRight(3);  
   goUp(2);
   goLeft(3);
   goUp(2);
   goRight(4);
   return 4;    
}
int drawK()
{
    goUp(5);
    moveDown(3);
    moveRight(1);
    goDiaUp(3);
    moveDownLeft(3);
    goDiaDown(3);
    return 4;

}
int drawE()
{
    goUp(4);
    goRight(5);
    moveLeft(5);
    moveDown(2);
    goRight(3);
    moveLeft(3);
    moveDown(2);
    goRight(5);
    return 5;
    
}
int drawN()
{
    goUp(4);
    goDiaDown(4);
    goUp(5);
    return 5;  
}

void createSnakeTitle(int x, int y)
{   
    int current_x = x;
    int current_y = y;
    int character_length;
    int spacing = 1;
    init_pair(4, COLOR_WHITE, COLOR_BLUE); // White text on blue background
    attron(COLOR_PAIR(4));
    character_length = drawS();
    current_x += character_length + spacing;
    cursor_x = current_x;
    cursor_y = current_y;
    character_length = drawN();
    current_x += character_length + spacing;
    cursor_x = current_x;
    cursor_y = current_y;
    character_length = drawA();
    current_x += character_length + spacing;
    cursor_x = current_x;
    cursor_y = current_y;
    character_length = drawK();
    current_x += character_length + spacing;
    cursor_x = current_x;
    cursor_y = current_y;
    character_length = drawE();
    current_x += character_length + spacing + 5;
    cursor_x = current_x;
    cursor_y = current_y; 
    attroff(COLOR_PAIR(4)); 
      
}

