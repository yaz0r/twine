OBJECTS = 3Dmodels.o actorScript.o actors.o angle.o anim.o body.o cdrom.o \
cfg.o comportementMenu.o costume.o decomp.o file.o fireEffect.o flaplayer.o \
font.o fullRedraw.o hqr.o images.o init.o input.o main.o mainLoop.o \
mainMenu.o mainSDL.o memory.o moveActor.o music.o object.o polyRender.o \
resource.o room.o save.o sdl.o sys.o text.o threadCode.o time.o vars.o \
video.o vox.o walk.o

CC = gcc

CXX = g++

CPPFLAGS = `sdl-config --cflags` -Wall -g

LDFLAGS = `sdl-config --libs` -lefence

lbarecode: ${OBJECTS}
	${CXX} ${LDFLAGS} ${OBJECTS} -o lbarecode

all: lbarecode

clean:
	rm -f ${OBJECTS} lbarecode
