#include "Multiplexer.h"

namespace PPUCore
{
	namespace Multiplexer
	{
		//TODO handle setting Sprite 0 hit flag

		//Format of each BG Pixel Byte:          Bit 3 and 2: BG Subpalette     Bit 1 and 0: BG Colour
		//Format of each Sprite Pixel Byte:      Bit7: Sprite 0 (if = 1)   Bit 6: Priority (0:OverBG 1:UnderBG)    Bit 4: 1 for any sprite pixel (ensure palette lookup is in last 4 sub-palettes)  Bit 3-0  SubPalette and Pixel Colour 
		void multiplexPixels(PPUState* state, int startingOffset, int endingOffset)
		{
			for (int i = startingOffset; i<= endingOffset; i++)
			{
				unsigned char resultPixel = 0;
				if (state->SpriteScanlineBuffer[i] == 0)
					resultPixel = state->BGScanlineBuffer[i];  //Sprite Pixel is transparent (no sprite pixel here) pass through background (this case occurs the most)
				else
				{
					//Some non-zero, possible sprite 0 pixel (could be transparent and/or priority flag is set)
										
					if ((state->SpriteScanlineBuffer[i] & 64) == 0)
					{
						//Sprite pixel over BG
						if (state->SpriteScanlineBuffer[i] & 0x3)
						{
							resultPixel = state->SpriteScanlineBuffer[i];

							if ((state->BGScanlineBuffer[i] & 3) != 0)
								if ((state->SpriteScanlineBuffer[i] & 128) && (state->scanlineOffset < 255)) 
									state->Sprite0HitFlag = 1;
						}
						else
							resultPixel = state->BGScanlineBuffer[i];
					}						
					else
					{
						//Sprite Pixel is "Under" BG  (rendered only where BG is the backdrop colour)
						if ((state->BGScanlineBuffer[i] & 0x3) == 0) //BG is transparent
							resultPixel = state->SpriteScanlineBuffer[i];
						else
						{
							resultPixel = state->BGScanlineBuffer[i];

							if ((state->SpriteScanlineBuffer[i] & 128) && ((state->SpriteScanlineBuffer[i] & 0x3) != 0) && (state->scanlineOffset < 255))
								state->Sprite0HitFlag = 1; //NOTE: if sprite pixel is non-transparent a Sprite 0 hit STILL occurs here (even though BG is over top of the non-transparent Sprite Pixel)
						}							
					}					
				}
				
				resultPixel &= 0x1F;  //only look at the colour bits (ignore Sprite 0 bit, etc for sprite pixels)

				//Read 32-Byte PAL Mem
				if ((resultPixel & 0x13) == 0) //For BG colour 0 pixels SELECT BACKDROP COLOUR (at $3F00/$3F10) not colour 0 in the sub-palette!
					resultPixel = 0;

				unsigned char palByte = state->PalMem[resultPixel & 0x1F];

				if (state->enableGrayscaleMode) palByte &= 0x30;

				state->ScanlineBuffer[i] = palByte;
			}
		}
	}
}