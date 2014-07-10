snake: main.o
	gcc -o snake main.o

main.o: main.c
	gcc -c main.c
