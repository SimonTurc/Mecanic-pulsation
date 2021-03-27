# Makefile

CPPFLAGS = -MMD `sdl2-config --cflags`
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0 epoxy` -Wall -O3
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0 epoxy` `sdl2-config --libs` -L/usr/X11R6/lib/ -lGL -lGLU -lGLEW -lglut -lm -lSDL2_mixer

SRC = main.c sound.c shape.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: main.o	
shape: shape.o
sound: sound.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} sound shape main

# END
