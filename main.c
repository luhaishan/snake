
/*
	This is a snake Game.
*/

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>

#define LEFT  -1
#define RIGHT 1
#define UP 2
#define DOWN -2

#define STATE_INIT 0
#define STATE_STARTED 1
#define STATE_GAMEOVER 2

#define WIDTH 80
#define HEIGHT 40

int orientation = RIGHT; /* the direction of snake */
int gameStatus = STATE_INIT;
int delay = 200; /* how long to wait */

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

void refreshSnake(); /* when time up, refresh snake*/
void doRefresh(); /* when time is up, do the fresh work */

bool isCollapse(); /* whether snake hit the wall */

int set_ticker(int n_msecs);

void initSnake();
void drawSnake();
void drawWelcomeMsg();
void drawPlayground();
void initGame();

int main(int argc, char* argv[])
{
	void onInput(int);
	void enable_kbd_signals();
    printf("This is a snake game!\n");
	initGame();
	int inputChar;
	while(1){
		inputChar = getch();
		if(inputChar == 32)
			break;
	}
	gameStatus = STATE_STARTED;
	initSnake();
	
	signal(SIGALRM, doRefresh);
	set_ticker(delay);
	signal(SIGIO, onInput);
	enable_kbd_signals();

		
	while(gameStatus != STATE_GAMEOVER){
		;
	}
	endwin();
    return 0;
}

void initGame()
{
	printf("Init the Game.\n");
	initscr();
	noecho();
	curs_set(0);
	drawPlayground();
    drawWelcomeMsg();
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

bool isCollapsed()
{
	int x = snake.head.x;
	int y = snake.head.y;

	if ( x <=0 || x>= HEIGHT-1 || y <=0 || y >= WIDTH-1 )
		return true;
	else
		return false;
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
	move(5, 25);
	printw("Press space to start!");
}

void doRefresh()
{
	clear();
	refreshSnake();
	bool b = isCollapsed();
	if(b){
		kill(getpid(), SIGINT);
	}
	drawPlayground();
	drawSnake();
	refresh();
}


/* snake tail always at the end of body array*/
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
		move(0, 0);
		printw("hello");
		break;
	default:
		break;
	}

	for (i = snake.bodyLength -1; i > 0; i--)
		snake.body[i] = snake.body[i-1];

	snake.body[0] = originalHead;
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

void onInput(int signum)
{
	int c = getch();
	int originalDir = orientation;
	if(c == 'w') 
	{
		orientation = UP;
		originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = UP);
	}
	if(c == 's')
	{
		orientation = DOWN;
		originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = DOWN);
	}
	if(c == 'a')
	{
		orientation = LEFT;
		originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = LEFT);
	}
	if(c == 'd')
	{
		orientation = RIGHT;
		originalDir + orientation == 0 ? (orientation = originalDir) : (orientation = RIGHT);
	}
	if(c == 'q')
	{
		/* here we want to quit the game */
		kill(getpid(), SIGINT);
	}
}

void enable_kbd_signals()
{
	int fd_flags;

	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags | O_ASYNC));
}

