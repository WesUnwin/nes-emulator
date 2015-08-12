#include "DrawSprites.h"

namespace PPUCore
{
	/**
	* Draws the 0-8 sprites in sprite temp memory to the sprite scanline buffer.
	* The sprite scanline buffer is later rasteristed with the BG scanline buffer (by the multiplexer) and used
	* to fill in the scanline buffer.
	*/
	void drawSprites(PPUState *state)
	{
		//Clear Sprite Scanline Buffer
		for (int i = 0; i < 256; i++)
			state->SpriteScanlineBuffer[i] = 0;

		for (int i = 7; i >= 0; i--)   //Sprites at lower indices in temp mem have higher priority (drawn overtop of higher index sprites)
		{
			if (state->SpritePixelData_Bitplane0[i] == 0 && state->SpritePixelData_Bitplane1[i] == 0) continue;

			unsigned char pixelByte = 0x10;  //enforces later 4 sub-palettes (for sprite pixels)
			if (state->SpritePixelData_IsSprite0[i] != 0) pixelByte |= 128;
			if (state->SpritePixelData_Priority[i] != 0) pixelByte |= 64;
			pixelByte |= ((state->SpritePixelData_Pal[i] & 0x3) << 2);

			int x = state->SpritePixelData_X[i];
			for (int p = 0; p < 8; p++)
			{
				unsigned char pixel = pixelByte | (((state->SpritePixelData_Bitplane1[i] >> p) & 0x1) << 1) | ((state->SpritePixelData_Bitplane0[i] >> p) & 0x1);
				if (pixel & 0x03) //Don't draw if its a transparent (see-through to other sprites/bg) pixel
					state->SpriteScanlineBuffer[x + p] = pixel;
			}
		}
	}


}