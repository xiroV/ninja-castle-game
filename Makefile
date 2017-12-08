CC = g++
LIBS = -lGL -lglut -lGLU -lGLEW
CFLAGS = -Wall

all:	game

game: main.o player.o map.o collision.o ai.o character.o loader/material.o
	$(CC) -o $@ main.o player.o map.o collision.o ai.o character.o material.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

loader/%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f game *.o loader/*.o
