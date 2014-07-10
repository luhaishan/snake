
/*
	This is a snake Game.
*/

#include <stdio.h>

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
	printf("-- Drawing a User Interface.\n");
	printf("-- There's a handler to handle user input\n");
	printf("-- Press the space button to start the Game.\n");
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
