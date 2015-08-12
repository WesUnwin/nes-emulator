#ifndef PPU_DRAWSPRITES_H
#define PPU_DRAWSPRITES_H

#include "PPUState.h"

namespace PPUCore
{
	
	/**
	* Draws the 0-8 Sprites in Sprite Temporary Memory onto the Sprite Scanline buffer
	*/
	void drawSprites(PPUState *state);


}

#endif