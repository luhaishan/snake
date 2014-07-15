snake: main.o
	gcc -o snake main.o -lcurses

main.o: main.c data.h
	gcc -c -ggdb main.c

clean:
	-rm snake main.o
