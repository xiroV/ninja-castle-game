CC = g++
LIBS = -lGL -lglut -lGLU -lGLEW
CFLAGS = -Wall

all:	ctf-game

ctf-game: main.o player.o map.o
	$(CC) -o $@ main.o player.o map.o $(LIBS)

#player: player.o
#	$(CC) -o $@ player.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f ctf-game *.o
