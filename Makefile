CC = g++
LIBS = -lGL -lglut -lGLU -lGLEW
CFLAGS = -Wall

all:	game

game: main.o player.o map.o collision.o ai.o
	$(CC) -o $@ main.o player.o map.o collision.o ai.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f game *.o
