
/*
	This is a snake Game.
*/

#include <stdio.h>
#include <curses.h>

void drawHorTopLine(int x, int y, int width);//draw the top horizon line
void drawHorBtmLine(int x, int y, int width);//draw the bottom horizon line
void drawLeftVerLine(int x, int y, int height);//draw the left vertical line
void drawRightVerLine(int x, int y, int height);//draw the right vertical line

void initWelcomeMsg();
void initPlayground();
void initGame();
void runGame();
void saveGame();

int main(int argc, char* argv[]) 
{
    printf("This is a snake game!\n");
	initGame();
	runGame();
	saveGame();
    return 0;
}

void initGame()
{
	printf("Init the Game.\n");
	//printf("-- Drawing a User Interface.\n");
	initscr();
	initPlayground();
    initWelcomeMsg();
	printf("-- There's a handler to handle user input\n");
	printf("-- Press the space button to start the Game.\n");
    refresh();
    getch();
    endwin();

}

void runGame()
{
	printf("The Game is running...\n");
	printf("-- A Handler to handle User input.\n");
	printf("-- Arrow Key to control the snake.\n");
	printf("-- q to exit the Game.\n");
}

void saveGame()
{
	printf("The Game is exiting, do some save stuff.\n");
	printf("-- At the very beginning , we may save nothing.\n");
}

void initPlayground()
{
	drawHorTopLine(0, 0, 80);
	drawHorBtmLine(39, 0, 80);
	drawLeftVerLine(0, 0, 40);
	drawRightVerLine(0, 79, 40);
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

void initWelcomeMsg()
{
	move(5, 30);
	printw("Press space to start!");
}
