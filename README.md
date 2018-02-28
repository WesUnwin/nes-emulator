## A Nintendo Entertainment System (NES) Emulator

# Introduction
A fun personal project I have being pursuing, both for the pure awesome-ness of learning about the inards of a great system, and at the same time furthering my skills in C++, and concepts in low-level computing and hardware.

Not intended to be an extremely accurate, nor stable, nor efficient implementation of an NES emulator, but purely as a fun project.

![alt text](https://github.com/WesUnwin/nes-emulator/blob/master/docs/images/screenshot.png)

# Building
Currently somewhat working (with no working input or sound yet) on linux. The emulator has been designed to separate platform
independent code (in src/core) as much as possible. Potentially not working on windows at the moment, but will be working there soon.

To build:

Install FLTK (and ensure header files are on include path for usage with g++).
http://www.fltk.org/software.php?VERSION=1.3.4&FILE=fltk/1.3.4/fltk-1.3.4-2-source.tar.gz

Then run: make

Then run: bin/linux/nes