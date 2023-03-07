# This is a Makefile
# or gcc if C instead if C++ use lstdc++ if using standard lib
CC = g++
LIBS = -lGL -lGLU -lglut -lm 

all: tess
# this is the linking line
tess: tess.o
	$(CC) tess.o -o tess $(LIBS)

#this is the compilation line

tess.o: tess.cpp
	$(CC) tess.cpp -c

