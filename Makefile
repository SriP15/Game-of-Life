CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: life

life: life.o universe.o
	$(CC) -o life life.o universe.o -lncurses

life.o: life.c universe.h
	$(CC) $(CFLAGS) -c life.c -o life.o

universe.o: universe.h
	$(CC) $(CFLAGS) -c universe.c -o universe.o

clean:
	rm -f life *.o

format:
	clang-format -i -style=file *.h
	clang-format -i -style=file *.c
