# Makefile

CPPFLAGS = -MMD `sdl2-config --cflags`
CC = gcc -pthread
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wextra -Wall -O3 -g 
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `sdl2-config --libs` -lGL -lGLEW -lSDL2_mixer  -lm

SRC = main.c sound.c shape.c movement.c sound_len.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: main.o shape.o movement.o sound.o sound_len.o
shape: shape.o
movement: movement.o
sound: sound.o
sound_len: sound_len.o
-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} sound shape movement main sound_len

# END
