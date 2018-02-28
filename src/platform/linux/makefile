FLTKCONFIG   = /media/user/DATA/projects/fltk-1.3.4-2/fltk-config
CXX          = $(shell $(FLTKCONFIG) --cxx)
CXXFLAGS     = $(shell $(FLTKCONFIG) --cxxflags) -Wall -g -I.
LINKFLTK     = $(shell $(FLTKCONFIG) --ldstaticflags)
#LINKFLTK_GL  = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)
#LINKFLTK_IMG = $(shell $(FLTKCONFIG) --use-images --ldstaticflags)
LINKFLTK_ALL = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)


CCFLAGS = -g

LINUX_FOLDER_OBJS = $(wildcard ../*.o)

all: nes

obj:
	mkdir -p obj

obj/GLRenderer.o: GLRenderer.cpp obj
	g++ $(CCFLAGS) -c $< -o $@

obj/GLWindow.o: GLWindow.cpp obj
	g++ $(CCFLAGS) -c $< -o $@

obj/LinuxSoundPlayer.o: LinuxSoundPlayer.cpp obj
	g++ $(CCFLAGS) -c $< -o $@

obj/LinuxInputCollector.o: LinuxInputCollector.cpp obj
	g++ $(CCFLAGS) -c $< -o $@

obj/LinuxKeyboardInputDevice.o: LinuxKeyboardInputDevice.cpp obj
	g++ $(CCFLAGS) -c $< -o $@

obj/main.o: main.cpp obj
	g++ $(CCFLAGS) -I/home/user/Desktop/fltk-1.3.3 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -Wall -w -c $< -o $@

nes: obj/main.o obj/GLRenderer.o obj/LinuxSoundPlayer.o obj/LinuxInputCollector.o obj/GLWindow.o obj/LinuxKeyboardInputDevice.o
	mkdir -p ../../../bin/linux
	$(CXX) obj/main.o obj/GLRenderer.o obj/LinuxInputCollector.o obj/LinuxSoundPlayer.o obj/GLWindow.o obj/LinuxKeyboardInputDevice.o $(LINKFLTK_ALL) ../../core/emu.o ../../core/cpu/libcpu.a ../../core/ppu/libppu.a ../../core/apu/libapu.a ../../core/input/libinput.a ../../core/cart/libcart.a -o ../../../bin/linux/nes
