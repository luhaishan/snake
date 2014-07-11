
/*
	This is a snake Game.
*/

#include <stdio.h>
#include <curses.h>
#include <termios.h>

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define STATE_INIT 0
#define STATE_STARTED 1
#define STATE_GAMEOVER 2

#define WIDTH 80
#define HEIGHT 40
 
int orientation; //the direction of snake
int gameStatus;
static struct termios initial_settings, new_settings; 

typedef struct {
	int x;
	int y;
}postion;

typedef struct {
	postion head;
	postion body[(WIDTH - 1) * (HEIGHT - 1)];
	int bodyLength;
} snakeType;

snakeType snake;

void drawHorTopLine(int x, int y, int width); //draw the top horizon line
void drawHorBtmLine(int x, int y, int width); //draw the bottom horizon line
void drawLeftVerLine(int x, int y, int height); //draw the left vertical line
void drawRightVerLine(int x, int y, int height); //draw the right vertical line

void doRefresh();
void waitForUserInput();

void initSnake();
void drawSnake();
void drawWelcomeMsg();
void drawPlayground();
void initGame();

void runGame();
void saveGame();

int main(int argc, char* argv[]) 
{
    printf("This is a snake game!\n");
	initGame();
	//runGame();
	saveGame();
    return 0;
}

void initGame()
{
	printf("Init the Game.\n");
	//printf("-- Drawing a User Interface.\n");
	int inputChar;
	initscr();
	noecho();
	curs_set(0);
	drawPlayground();
    drawWelcomeMsg();

	printf("-- There's a handler to handle user input\n");
	while (1) {
		inputChar = getch();
		if (inputChar == 'q') 
			break;
		if (inputChar == 32)
			runGame(); 
	}
    endwin();

}

void runGame()
{
	initSnake();
	doRefresh();
	//printf("The Game is running...\n");
	//printf("-- A Handler to handle User input.\n");
	//printf("-- Arrow Key to control the snake.\n");
	//printf("-- q to exit the Game.\n");
}

void saveGame()
{
	//printf("The Game is exiting, do some save stuff.\n");
	//printf("-- At the very beginning , we may save nothing.\n");
}

void drawPlayground()
{
	drawHorTopLine(0, 0, WIDTH);
	drawHorBtmLine(HEIGHT-1, 0, WIDTH);
	drawLeftVerLine(0, 0, HEIGHT);
	drawRightVerLine(0, WIDTH-1, HEIGHT);
}

void drawHorTopLine(int x, int y, int width)
{
	move(x, y);
	int i = 0;
	while (i < width) {
		printw("x");
		i++;
	}
}

void drawHorBtmLine(int x, int y, int width)
{
    move(x, y);
    int i = 0;
    while (i < width) {
        printw("x");
        i++;
    }
}

void drawLeftVerLine(int x, int y, int height)
{
    int i = 0;
    while (i < height) {
		move(x+i, y);
        printw("x");
        i++;
    }
}

void drawRightVerLine(int x, int y , int height)
{
    int i = 0;
    while (i < height) {
		move(x+i, y);
        printw("x");
        i++;
    }
}

void drawWelcomeMsg()
{
	move(5, 30);
	printw("Press space to start!");
}

void doRefresh()
{
	clear();
	drawPlayground();
	drawSnake();
	refresh();
}


void initSnake()
{
	int i;
	snake.head.x = 4;
	snake.head.y = 20;
	snake.bodyLength = 3;
	
	for (i = 0; i < snake.bodyLength; i++) {
		snake.body[i].x = snake.head.x;
		snake.body[i].y =  snake.head.y - i - 1;
	}
}

void drawSnake()
{
	int i;
	int x;
	int y;
	x = snake.head.x;
	y = snake.head.y;
	move(x, y);
	printw("@");

	for (i = 0; i< snake.bodyLength; i++) {
		x = snake.body[i].x;
		y = snake.body[i].y;
		move(x, y);
		printw("O");
	}
}
