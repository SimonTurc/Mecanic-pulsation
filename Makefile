CPPFLAGS = -MMD `sdl2-config --cflags`
CC = gcc -pthread
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wextra -Wall -O3 -g 
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `sdl2-config --libs` -lGL -lGLEW -lSDL2_mixer  -lm

vpath %.c
SRC := $(wildcard src/*.c)
DEP = ${SRC:.c=.d}
EXEC = mp
OBJDIR = dist
OBJ= $(addprefix $(OBJDIR)/, movement.o shape.o sound_len.o sound.o main.o)

all: $(EXEC)

$(EXEC) : ${OBJ}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJ} $(LDLIBS)
dist/movement.o: src/movement.c
	$(CC) $(CFLAGS) -c -o dist/movement.o src/movement.c
dist/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o dist/main.o src/main.c
dist/sound_len.o: src/sound_len.c
	$(CC) $(CFLAGS) -c -o dist/sound_len.o src/sound_len.c
dist/shape.o: src/shape.c
	$(CC) $(CFLAGS) -c -o dist/shape.o src/shape.c
dist/sound.o: src/sound.c
	$(CC) $(CFLAGS) -c -o dist/sound.o src/sound.c

-include ${DEP}


.PHONY: clean

clean:
	rm -f dist/*.o src/*.d mp
