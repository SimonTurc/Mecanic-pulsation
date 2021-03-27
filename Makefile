# Makefile

CPPFLAGS = -MMD `sdl2-config --cflags`
CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wextra -Wall -O3
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `sdl2-config --libs` -lGL -lGLEW -lSDL2_mixer

SRC = main.c sound.c shape.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: main.o shape.o
shape: shape.o
sound: sound.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} sound shape main

# END
