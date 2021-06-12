CPPFLAGS = -MMD `sdl2-config --cflags`
CC = gcc -pthread -fsanitize=address
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wextra -Wall -O3 -g 
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `sdl2-config --libs` -lGL -lGLEW -lSDL2_mixer  -lm

vpath %.c
SRC := $(wildcard src/*.c)
DEP = ${SRC:.c=.d}
EXEC = mp
OBJDIR = obj
OBJ= $(addprefix $(OBJDIR)/, shape_create.o movement.o shape.o sound_len.o sound.o main.o)

all: $(OBJDIR) $(EXEC)

$(EXEC) : ${OBJ}
	$(CC) $(CFLAGS) -o $(EXEC) ${OBJ} $(LDLIBS)
obj/movement.o: src/movement.c
	$(CC) $(CFLAGS) -c -o obj/movement.o src/movement.c
obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o obj/main.o src/main.c
obj/sound_len.o: src/sound_len.c
	$(CC) $(CFLAGS) -c -o obj/sound_len.o src/sound_len.c
obj/shape.o: src/shape.c
	$(CC) $(CFLAGS) -c -o obj/shape.o src/shape.c
obj/sound.o: src/sound.c
	$(CC) $(CFLAGS) -c -o obj/sound.o src/sound.c
obj/shape_create.o: src/shape_create.c
	$(CC) $(CFLAGS) -c -o obj/shape_create.o src/shape_create.c

$(OBJDIR):
	mkdir -p $(OBJDIR)

-include ${DEP}


.PHONY: clean

clean:
	rm -rf obj src/*.d mp
