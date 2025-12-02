#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define INITIAL_LENGTH 3//would define the initial length of the snake
#define max_snake_length 1000 // max snake length
#define START_TIME_DELAY 100000 // starting time delay
#define FOOD_DECREMENT 500 // the amount of time the food decrements
#define FOOD_SCORE_INCREMENT 2 // the points for each food

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
int running = 1;

//the game would have to interact with the snake, the border,the direction etc
//there fore they would become global in order to not pass so many parameters
void draw();
void init_game();
void gameOverScreen();

void resize_handler(int);
void sigint_handler(int);
void moveSnake();
int collides_with_wall();
int collides_with_snake(int,int);
int collides_with_self();
void place_food();
void createSnakeTitle(int,int);
void throw_title_screen();
//for snake direction
enum GAME_OVER_REASON
{
    BORDER_HIT,
    SELF_HIT,
    QUIT,
    WIN
}reason;
enum DIRECT
{
    Up,
    Down,
    Left,
    Right,
    Stop
}direction;

//the main method
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
    signal(SIGWINCH,resize_handler);
    signal(SIGINT,sigint_handler);
    init_game();
    nodelay(stdscr,TRUE);

    
    int key;     
    while(running)
    {
       
        key = getch();
        if(key != ERR)
        {
            if(key == 'q' || key == 'Q') //for quitting
            {break;}
            switch(key)//changes direction from arrow keys
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
        
        moveSnake();//moves the snake
        draw();//draws content
        
        //check for collision with wall
        if(collides_with_wall())
        {
            reason = BORDER_HIT;
            gameOverScreen();
            continue;
        }
        //checks for collision with self
        if(collides_with_self())
        {    
            reason = SELF_HIT;
            gameOverScreen();
            continue;
        }
        //top score
        if(score >= top_score)
        {
            reason = WIN;
            gameOverScreen();
            continue;
        }  


        //check for food 
        if(snake[0].x == food.x && snake[0].y == food.y)
        {
            snake_len++;
            score += FOOD_SCORE_INCREMENT;
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
        //making the up and down movements a little slower 
        if(direction == Up || direction == Down)
        {
            usleep(delay+1000);
        }
        else
        {
          usleep(delay);
        }


        
    }
    //game quit
    nodelay(stdscr,FALSE);
    mvprintw(field_bottom + 2, field_left, "Thanks for playing! press any key to exit...");
    getch();
    endwin();   
    return 0;
}


//Tyris - would change the direction of the snake and apply change in positioning of the snake head and body 
void moveSnake()
{
    if(direction == Stop) // If the snake has not started moving yet
    {
        return; // do nothing and exit the function
    }
    for(int i = snake_len-1 ;i >= 1; --i) // Move each body segment to the position of the segment in front of it
    {
        snake[i] = snake[i-1]; // body follows the previous segment 
    }
    switch(direction) // Move the head based on the current direction
    {
        case Up:
            snake[0].y -= 1;// Move head up (decreasing y)
            break;
        case Down:
            snake[0].y += 1; // Move head up (increase y)
            break;
        case Left:
            snake[0].x -= 1; // Move head left (decrease x)
            break;
        case Right:
            snake[0].x += 1; // Move head right (increasing x)
            break;
        default:
    }
}
//Erick - this function increases the size of the border if the terminal is moved using the SIGWINCH signal
void resize_handler(int sig)
{
    endwin();
    refresh();
    clear();

    int max_x = COLS;
    int max_y = LINES;
    
    field_top = 1;
    field_left= 1;
    field_bottom = max_y - 4;
    field_right = max_x - 1;
    draw();
}
//Erick - this hnadler is for when the games exits from the SIGINT signal
void sigint_handler(int sig)
{
    running = 0;
}
//Erick
// function for drawing the border, snake, scoreboard and instructions. This function would esentially present the whole game 
void draw()
{
    clear();//first clears off old content

   //for drawing border
    init_pair(1,COLOR_BLUE,COLOR_BLUE);//color for the border
    attron(COLOR_PAIR(1));
    for(int x = field_left; x <= field_right;++x)//x border fields
    {
        mvaddch(field_top,x,' ');
        mvaddch(field_bottom,x,' ');
    }
    for(int y = field_top; y <= field_bottom; ++y)//y border fields
    {
       mvaddch(y,field_left,' ');
       mvaddch(y,field_right,' '); 
    }
    attroff(COLOR_PAIR(1));
  //draw the score
   
    mvprintw(field_bottom + 1,field_left,"Score = %d  HighScore = %d Top score = %d ",score,high_score,top_score);//score board
    mvprintw(field_bottom + 2,field_left,"Controls: arrow keys | q to quit");//instructions for the controls
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
    for(int i = 1; i < snake_len; ++i)//for the positioning of the snake body
    {
        attron(COLOR_PAIR(3));
       mvaddch(snake[i].y,snake[i].x,'o');
       attroff(COLOR_PAIR(3));
    }
    //for drawing the food
    mvaddch(food.y,food.x,'O');

    refresh();//refreshes the whole change
}
// Tyris- places food
void place_food()
{
    int x,y; // variables to hold random cords 
    do{
        x = (rand() % (field_right - field_left - 1)) + field_left + 1; // Pick a random X inside the field boundaries
        y = (rand() % (field_bottom - field_top - 1)) + field_top + 1;  // Pick a random Y inside the field boundaries    
    } while(collides_with_snake(x,y)); // Keep trying if food spawns on the snake
    food.x = x; // Save final food X position
    food.y = y; // Save final food Y position 
}


//Tyris- checks if there was wall collison If the snake’s head touches the border of the game field, the game ends immediately.
int collides_with_wall()
{
    if(snake[0].x <= field_left || // Head touches or passes left wall
       snake[0].x >= field_right || // Head touches or passes right wall
       snake[0].y <= field_top || // Head touches or passes top wall
       snake[0].y >= field_bottom) // head touches or passes bottom wall
    {
        return 1; // collision detected
    }
    return 0; // No collosion 
}
//for getting the input of user during game pausing


//Tyris- checks if the cords collide with the snake(used for food)
int collides_with_snake(int x,int y)
{
    for(int i = 0; i < snake_len; ++i) // Loop through every part of the snake
    {
        if(snake[i].x == x && // Compare X position
           snake[i].y == y) // Compare Y position 
        {
            return 1; // maych found = collision
        }
    }
    return 0; // no collision 
}


//Tyris- checks for collision with the snake itself
int collides_with_self()
{
    for(int i = 1; i < snake_len; ++i) // start at 1 (skip head), check all body segments 
    {
        if(snake[0].x == snake[i].x && // Head X equals a body X 
           snake[0].y == snake[i].y) // Head Y equals a body Y
        {
            return 1; // collision with itself
        }
    }
    return 0; // no collision 
}


//Erick - this function would initialize everything for the game, which is good for starting and resetting the game
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


void gameOverScreen() {
    nodelay(stdscr,FALSE);
    clear();

    switch (reason) {
        case BORDER_HIT: mvprintw(8, 5, "GAME OVER : You hit the wall!"); break;
        case SELF_HIT:   mvprintw(8, 5, "GAME OVER : You ran into yourself!"); break;
        case WIN:        mvprintw(8, 5, "YOU WIN! Congratulations!"); break;
        default:         mvprintw(8, 5, "GAME OVER"); break;
    }

    mvprintw(10, 5, "Final Length: %d", snake_len);
    mvprintw(12, 5, "Score : %d", score);
    mvprintw(14, 5, "High Score : %d", high_score);
    mvprintw(16, 5, "Press R to Retry");
    mvprintw(18, 5, "Press Q to Exit");

    refresh();

    int ch;
    nodelay(stdscr,TRUE);
    while((ch = getch()))
    {
         if (ch == 'r' || ch == 'R')
         {
            init_game();
            return;
         }
          if (ch == 'q' || ch == 'Q')
          {
            raise(SIGINT);
            return;
          }
    }
        
    
}

//Erick - this functions is for bringing out the title screen on the start of the game
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
//Erick
//these were functions created in assissting and creating the title screen
void moveRight(int dist)//moves cursor right
{
    for(int i = 0; i < dist; ++i)//move right
   {
    cursor_x++;//right
   }
}
void moveLeft(int dist)//moves cursor left
{
   for(int i = 0; i < dist; ++i)//move right
   {
    cursor_x--;//left
   }
}
void moveDown(int dist)//moves cursor down
{
    for(int i = 0; i < dist;++i)
    {
        cursor_y++;//down
    }
}
void moveUp(int dist)//moves cursor up
{
    for(int i = 0; i < dist;++i)
    {
        cursor_y--;//up
    }
}
void moveDownLeft(int dist)//moves cursor down and left
{
    for(int i = 0; i < dist;++i)
    {
        cursor_x--;//left
        cursor_y++;//down
    }
}
void moveUpLeft(int dist)//moves cursor up and left
{
    for(int i = 0; i < dist;++i)
    {
        cursor_x--;//left
        cursor_y--;//Up
    }
}


//functions for drawing with the cursor

void goLeft(int dist)//draw left
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x--;
    refresh();
   }
}
void goRight(int dist)//draw right
{
   for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    refresh();
    }
}
void goUp(int dist)//draw up
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_y--;
    refresh();
   }
}
void goDown(int dist)//draw down
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_y++;
    refresh();
    }
}
void goDiaDown(int dist)//up down diagonally
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    cursor_y++;
    refresh();
    }
}
void goDiaUp(int dist)//draw up diagonally
{
    for(int i = 0; i < dist; ++i)//move right
   {
    
    mvprintw(cursor_y,cursor_x," ");
    cursor_x++;
    cursor_y--;
    refresh();
    }
}
int drawA()//draws an A
{
   goDiaUp(4);
   goDiaDown(5);
   moveUpLeft(3);
   goLeft(4);
   return 9;   
}

int drawS()//draws an S
{   
   goRight(3);  
   goUp(2);
   goLeft(3);
   goUp(2);
   goRight(4);
   return 4;    
}
int drawK()//draws a K
{
    goUp(5);
    moveDown(3);
    moveRight(1);
    goDiaUp(3);
    moveDownLeft(3);
    goDiaDown(3);
    return 4;

}
int drawE()//draws an E
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
int drawN()//draws N
{
    goUp(4);
    goDiaDown(4);
    goUp(5);
    return 5;  
}

void createSnakeTitle(int x, int y)//creates the whole snake title
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

