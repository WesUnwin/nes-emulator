TOP := $(realpath ../)

CC = g++

CFLAGS = -Wall

all: core linux

core:
	cd src/core && $(MAKE)
	
linux: core
	cd src/platform/linux && $(MAKE)
	
clean:
	@echo "Deleting object files..."
	find . -type f -name '*.o' -delete
	find . -type f -name '*.a' -delete