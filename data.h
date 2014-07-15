#ifndef _DATA_H_
#define _DATA_H_

/* direction */
#define LEFT  	-1
#define RIGHT 	1
#define UP 	2
#define DOWN 	-2

/* game status */
#define STATE_INIT 	0
#define STATE_STARTED 	1
#define STATE_GAMEOVER 	2
#define STATE_PAUSE	3

/* playground size*/
#define WIDTH 	40
#define HEIGHT 	20

/* cell type */
#define BLANK 		0
#define SNAKE_HEAD 	1
#define SNAKE_BODY 		2
#define FRUIT 		3
#define WALL 		4

typedef struct {
	int x;
	int y;
}postion;

typedef struct {
	postion head;
	postion body[(WIDTH - 2) * (HEIGHT - 2)];
	int bodyLength;
} snakeType;


#endif
