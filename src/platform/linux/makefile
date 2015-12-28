ifeq '$(OS)' "Windows_NT"
EXEEXT = .exe
endif

FLTKCONFIG   = /home/user/Desktop/fltk-1.3.3/fltk-config
CXX          = $(shell $(FLTKCONFIG) --cxx)
CXXFLAGS     = $(shell $(FLTKCONFIG) --cxxflags) -Wall -I.
LINKFLTK     = $(shell $(FLTKCONFIG) --ldstaticflags)
#LINKFLTK_GL  = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)
#LINKFLTK_IMG = $(shell $(FLTKCONFIG) --use-images --ldstaticflags)
LINKFLTK_ALL = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)
.SUFFIXES: .cxx .h .fl .o $(EXEEXT)

LINUX_FOLDER_OBJS = $(wildcard ../*.o)

all: nes

obj:
	mkdir -p obj

obj/GLRenderer.o: GLRenderer.cpp obj
	g++ -c $< -o $@

obj/GLWindow.o: GLWindow.cpp obj
	g++ -c $< -o $@

obj/LinuxSoundPlayer.o: LinuxSoundPlayer.cpp obj
	g++ -c $< -o $@

obj/LinuxInputCollector.o: LinuxInputCollector.cpp obj
	g++ -c $< -o $@

obj/main.o: main.cpp obj
	g++ -I/home/user/Desktop/fltk-1.3.3 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -Wall -w -c $< -o $@

nes: obj/main.o obj/GLRenderer.o obj/LinuxSoundPlayer.o obj/LinuxInputCollector.o obj/GLWindow.o
	$(CXX) obj/main.o obj/GLRenderer.o obj/LinuxInputCollector.o obj/LinuxSoundPlayer.o obj/GLWindow.o $(LINKFLTK_ALL) ../../core/emu.o ../../core/cpu/libcpu.a ../../core/ppu/libppu.a ../../core/apu/libapu.a ../../core/input/libinput.a ../../core/cart/libcart.a -o nes
