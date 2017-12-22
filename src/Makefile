CC = g++
LIBS = -L/usr/local/lib -lGL -lGLU -lXrandr -lXxf86vm -lXi -lX11 glew/lib64/libGLEW.a freeglut/lib/libglut.a
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
