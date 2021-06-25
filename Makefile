# Makefile
FLAGS = -std=c99 -Wall -O1

life: life.c
	gcc ${FLAGS} -o life life.c

clean:
	rm -f life