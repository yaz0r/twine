CC ?= gcc
CFLAGS=-I /usr/include/SDL/ -g -Wall -Wextra -Werror
LDFLAGS=-lSDL -lSDL_mixer -g
OBJ=actors.o \
	actorScript.o \
	angle.o \
	angleTable.o \
	animAction.o \
	anim.o \
	body.o \
	comportementMenu.o \
	costume.o \
	cube.o \
	extra.o \
	fichePerso.o \
	fireEffect.o \
	fla.o \
	font.o \
	fullRedraw.o \
	hqr.o \
	images.o \
	input.o \
	inventory.o \
	script.o \
	main.o \
	mainLoop.o \
	mainMenu.o \
	mainSDL.o \
	moveActor.o \
	music.o \
	object.o \
	overlay.o \
	renderer.o \
	room.o \
	samples.o \
	save.o \
	shadow.o \
	streamReader.o \
	text.o \
	time.o \
	vars.o \
	vox.o \
	zones.o

TARGET=twin-e

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ)

mrproper: clean
	rm $(TARGET)
