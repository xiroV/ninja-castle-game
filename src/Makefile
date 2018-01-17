CC = g++
LIBS = -L/usr/local/lib -lGL -lGLU -lX11 -lGLEW -lglut
CFLAGS = -Wall

all:	game

game: main.o player.o map.o core.o ai.o character.o loader/material.o
	$(CC) -o ../bin/$@ main.o player.o map.o core.o ai.o character.o material.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

loader/%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f ../bin/game *.o loader/*.o
