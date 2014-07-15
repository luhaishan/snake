
/*
	This is a snake Game.
*/

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>

#include "data.h"

int orientation = RIGHT; 		/* the direction of snake */
int gameStatus = STATE_INIT; 	/* game status 0*/
static struct termios initial_settings, new_settings;
int cell[WIDTH * HEIGHT];
snakeType snake; 	/* the snake */
int fruit; 			/* postion of fruit */
int fruitNum = 0; 	/* number of fruit */
int delay = 200;                /* how long to wait */
int set_ticker(int n_msecs);

void drawCell();
void drawWelcomeMsg();
void drawPauseMsg();
void drawGameoverMsg();

int addFruit();
void initCell();
void initSnake();
void initWall();
void initGame();

int main(int argc, char* argv[])
{
	void onInput(int);
	void onTime();
	void enable_kbd_signals();

	initscr();
	noecho();
	curs_set(0);
	srand((int) time(0));

	initGame();
	drawCell();
	drawWelcomeMsg();

	/* press space to start the game */
	int inputChar;
	while(1){
		inputChar = getch();
		if(inputChar == 32)
			break;
	}

	gameStatus = STATE_STARTED;
	//drawCell();
	
	signal(SIGALRM, onTime);
	set_ticker(delay);
	signal(SIGIO, onInput);
	enable_kbd_signals();

	while(1){
	}

	endwin();
	return 0;
}


int addFruit()
{
	int num = 0;
	int i = 0;
	int pos = 0;
	int max = (WIDTH-2)*(HEIGHT-2)-snake.bodyLength;
	num = rand()%max;
	for(i = 0, pos = 0; i < WIDTH * HEIGHT; i++)
	{
		if(cell[i] == BLANK)
			pos++;
		if (pos == num)
			return i;
	}
}

void initWall()
{
	int i = 0;
	for(i=0; i<WIDTH; i++)
	{
		cell[i] = WALL;
		cell[i + WIDTH * (HEIGHT - 1)] = WALL;
	}
	for(i=0; i < HEIGHT; i++)
	{
		cell[i * WIDTH] = WALL;
		cell[(i + 1) * WIDTH - 1] = WALL;
	}
}

void initCell()
{
	int i;
	for(i = 0; i< WIDTH * HEIGHT; i++)
	{
		cell[i] = BLANK;
	}

}

void initGame()
{
	/* init the cell*/
	initCell();

	/* init the wall */
	initWall();

	/* init the snake */
	initSnake();

	/* init the fruit */
	if(fruitNum == 0){
		int pos = addFruit();
		cell[pos] = FRUIT;
		fruitNum++;
	}
}

void drawCell()
{
	int x, y;
	for(x = 0; x < HEIGHT; x++)
	{
		for(y = 0; y < WIDTH; y++)
		{
			move(x, y);
			switch(cell[x * WIDTH + y])
			{
				case BLANK:
					//do nothing
					break;
				case SNAKE_HEAD:
					printw("@");
					break;
				case SNAKE_BODY:
					printw("O");
					break;
				case WALL:
					printw("X");
					break;
				case FRUIT:
					printw("F");
					break;
				default:
					break;
			}
		}
	}
}

void drawWelcomeMsg()
{
	move(10, 10);
	printw("Press space to start!");
}

void drawPauseMsg()
{
	move(10, 2);
	printw("Game paused, press space to resume.");
}

void drawGameoverMsg()
{
	move(10, 2);
	printw("Game over, press space to retry");
}

/* snake tail always at the end of body array*/
void initSnake()
{
	int i;
	snake.head.x = 4;
	snake.head.y = 20;
	snake.bodyLength = 3;

	cell[snake.head.x * WIDTH + snake.head.y] = SNAKE_HEAD;

	for (i = 0; i < snake.bodyLength; i++) {
		snake.body[i].x = snake.head.x;
		snake.body[i].y =  snake.head.y - i - 1;
		cell[snake.body[i].x * WIDTH + snake.body[i].y] = SNAKE_BODY;
	}
}

void refreshSnake()
{
	postion originalHead = snake.head;
	int i = 0;
	switch(orientation){
	case LEFT:
		snake.head.y -= 1; 
		break;
	case RIGHT:
		snake.head.y += 1;
		break;
	case UP:
		snake.head.x -= 1;
		break;
	case DOWN:
		snake.head.x += 1;
		break;
	default:
		break;
	}

	/* hit the wall or snake body */
	if(cell[snake.head.x * WIDTH + snake.head.y] == WALL 
		|| cell[snake.head.x * WIDTH + snake.head.y] == SNAKE_BODY)
	{
		gameStatus = STATE_GAMEOVER;
	}
	else if(cell[snake.head.x * WIDTH + snake.head.y] == FRUIT)
	{
		/* snake eat the fruit */
		for (i = snake.bodyLength; i > 0; i--)
			snake.body[i] = snake.body[i-1];

		snake.body[0] = originalHead;
		cell[snake.body[0].x * WIDTH + snake.body[0].y] = SNAKE_BODY;
		cell[snake.head.x * WIDTH + snake.head.y] = SNAKE_HEAD;

		snake.bodyLength++;

		/* fruit has been eaten, create new fruit */
		fruitNum--;
		if (fruitNum == 0){
			cell[addFruit()] = FRUIT;
			fruitNum ++;
		}
	}
	else if (cell[snake.head.x * WIDTH + snake.head.y] == BLANK)
	{
		cell[snake.head.x * WIDTH + snake.head.y] = SNAKE_HEAD;
		int len = snake.bodyLength;		
		postion tail = snake.body[len - 1];
		for (i = snake.bodyLength -1; i > 0; i--)
			snake.body[i] = snake.body[i-1];
		
		snake.body[0] = originalHead;
		cell[snake.body[0].x * WIDTH + snake.body[0].y] = SNAKE_BODY;

		cell[tail.x * WIDTH + tail.y] = BLANK;
	}
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec,n_usecs;

	n_sec = n_msecs/1000;
	n_usecs = (n_msecs%1000)*1000L;

	new_timeset.it_interval.tv_sec=n_sec;
	new_timeset.it_interval.tv_usec=n_usecs;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec= n_usecs;

	return setitimer(ITIMER_REAL,&new_timeset,NULL);
}

void onTime()
{
	if(gameStatus == STATE_PAUSE)
		return;

	if(gameStatus == STATE_GAMEOVER){
		clear();
		drawCell();
		drawGameoverMsg();
		refresh();
		return;
	}

	if(gameStatus == STATE_INIT){
		orientation = RIGHT;
		fruitNum = 0;
		initCell();
		initWall();
		initSnake();
		clear();
		drawCell();
		drawWelcomeMsg();
		refresh();
		return;
	}

	if(fruitNum == 0){
		int pos = addFruit();
		cell[pos] = FRUIT;
		fruitNum++;
	}

	clear();

	refreshSnake();
	drawCell();

	refresh();
}

void onInput(int signum)
{
	int c = getch();

	if(c == 'q'){
		/* here we want to quit the game */
		kill(getpid(), SIGINT);
	}
 
	if(gameStatus == STATE_STARTED){
		int originalDir = orientation;
		if(c == 'w'){
			orientation = UP;
			originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = UP);
		}
		if(c == 's'){
			orientation = DOWN;
			originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = DOWN);
		}
		if(c == 'a'){
			orientation = LEFT;
			originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = LEFT);
		}
		if(c == 'd'){
			orientation = RIGHT;
			originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = RIGHT);
		}
		if(c == 'p'){
			gameStatus = STATE_PAUSE;
			drawPauseMsg();
		}
	}
	else if(gameStatus == STATE_PAUSE){
		if(c == 32)
			gameStatus = STATE_STARTED;
	}
	else if(gameStatus == STATE_GAMEOVER){
		if(c == 32)
			gameStatus = STATE_INIT;
	}
	else if(gameStatus == STATE_INIT){
		if(c == 32)
			gameStatus = STATE_STARTED;
	}
}

void enable_kbd_signals()
{
	int fd_flags;

	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags | O_ASYNC));
}

