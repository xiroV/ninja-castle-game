CC = g++
LIBS = -L/usr/local/lib -lGL -lGLU -lGLEW -lglut
CFLAGS = -Wall

all:	ninjacastle

ninjacastle: main.o player.o map.o core.o ai.o character.o loader/material.o
	$(CC) -o ../bin/$@ main.o player.o map.o core.o ai.o character.o material.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

loader/%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f ../bin/ninjacastle *.o loader/*.o
