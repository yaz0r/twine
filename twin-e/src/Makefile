OBJECTS = 3Dmodels.o actorScript.o actors.o body.o cdrom.o cfg.o costume.o \
decomp.o file.o fireEffect.o flaplayer.o font.o fullRedraw.o hqr.o images.o \
init.o input.o main.o mainLoop.o mainMenu.o mainSDL.o memory.o music.o \
polyRender.o resource.o room.o save.o sdl.o sys.o text.o time.o vars.o \
video.o vox.o threadCode.o

CC = gcc

CXX = g++

CPPFLAGS = `sdl-config --cflags` -Wall -g

LDFLAGS = `sdl-config --libs` -lefence

lbarecode: ${OBJECTS}
	${CXX} ${LDFLAGS} ${OBJECTS} -o lbarecode

all: lbarecode

clean:
	rm -f ${OBJECTS} lbarecode

