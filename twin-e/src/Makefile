OBJECTS = 3Dmodels.o actorScript.o actors.o angle.o anim.o body.o cdrom.o \
cfg.o comportementMenu.o costume.o decomp.o debug.o file.o fireEffect.o flaplayer.o \
font.o fullRedraw.o hqr.o images.o init.o input.o main.o mainLoop.o \
mainMenu.o mainSDL.o memory.o moveActor.o music.o object.o polyRender.o \
renderer.o resource.o room.o save.o sdl.o sys.o text.o threadCode.o time.o vars.o SDL_rotozoom.o\
video.o vox.o 

CC = gcc

CXX = g++

CPPFLAGS = `sdl-config --cflags` -Wall -g -DFASTDEBUG 

LDFLAGS = `sdl-config --libs` -lSDL_ttf 

EFENCEFLAGS = -lefence

release: CPPFLAGS += -O3
release: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o lbarecode

efence: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(EFENCEFLAGS) $(OBJECTS) -o lbaefence


all: release 

clean:
	rm -f $(OBJECTS) lbarecode lbaefence
