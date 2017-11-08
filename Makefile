CC = g++
LIBS = -lGL -lglut -lGLU -lGLEW
CFLAGS = -Wall

all:	test1

test1: test1.o

	$(CC) -o $@ test1.o $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f test1 *.o
