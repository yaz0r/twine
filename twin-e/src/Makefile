OBJECTS = actorScript.o actors.o angle.o anim.o body.o cdrom.o \
cfg.o comportementMenu.o costume.o cube.o decomp.o debug.o extra.o expand.o file.o fireEffect.o flaplayer.o \
font.o fullRedraw.o global.o hqr.o HQMGlobal.o HQMLib.o HQRlib.o images.o init.o input.o inventory.o main.o mainLoop.o \
mainMenu.o mainSDL.o memory.o moveActor.o music.o object.o \
renderer.o resource.o room.o samples.o save.o sdl.o SDL_gfxPrimitives.o shadow.o streamReader.o sys.o text.o threadCode.o time.o\
vars.o SDL_rotozoom.o video.o vox.o zones.o 

CC = gcc

CXX = g++

CPPFLAGS = `sdl-config --cflags` -Wall -g -DUNIX -DPRELOAD_ALL 

LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_mixer 

EFENCEFLAGS = -lefence

release: CPPFLAGS += -O3
release: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o lbarecode

efence: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(EFENCEFLAGS) $(OBJECTS) -o lbaefence


all: release 

clean:
	rm -f $(OBJECTS) lbarecode lbaefence
