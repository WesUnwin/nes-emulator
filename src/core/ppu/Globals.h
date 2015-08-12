#ifndef PPU_GLOBALS_H
#define PPU_GLOBALS_H



struct SCANLINEBUFFER
{
	unsigned char pixels[255]; //The index into the master palette of each of the 256 visible pixels on a scanline
};

#endif