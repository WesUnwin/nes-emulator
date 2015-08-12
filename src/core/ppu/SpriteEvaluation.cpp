#include "SpriteEvaluation.h"

#include "PPUState.h"

namespace PPUCore
{
	namespace SpriteEvaluations
	{
		//Looks through all sprites in the 64-byte sprite mem, selects the first 8 (or up to 8)
		//that are in range of the current scanline, and adds them into sprite temp memory
		void doSpriteEvaluation(PPUState *state, int spriteNum)
		{
			if (state->scanlineNumber < 21) return;

			int visibleScanline = state->scanlineNumber - 21;
			
			int s = spriteNum * 4; //Index into sprite mem, of first of the 4 bytes for the sprite being evaluated
			
			int y = state->SpriteMem[s]; 

			int yDelta = (visibleScanline - y);

			bool inRange = (yDelta >= 0) && ((state->SpriteSize8x16 == 0) ? (yDelta < 8) : (yDelta < 16));

			if ( inRange )
			{
				//Sprite is in range
				if (state->SpritesOnScanline >= 8)
				{
					//All 8 Sprite Temp Mem Slots already filled
					state->SpriteOverflowFlag = 1;
					return;
				}

				state->SpritesOnScanline++;

				//Fill in Temp Mem with Sprites' Info

				if (state->SpriteMem[s + 2] & 128)  //V Flip set for Sprite
				{
					if (state->SpriteSize8x16 == 0)
						yDelta = 7 - yDelta;   //Vertical flip of row #, for 8x8 sprites
					else
						yDelta = 15 - yDelta;  //Vertical flip of row #, for 8x16 sprites
				}

				int t = state->SpritesOnScanline - 1;  //Index into sprite temp mem (slot being filled in: 0-8)

				state->SpriteTempMem_YDelta[t] = yDelta;   //4-bit (visibleScanline - y) comparator result   (inverted if V Flip was set)
					
				state->SpriteTempMem_Pattern[t] = state->SpriteMem[s + 1];

				state->SpriteTempMem_Pal[t] = (state->SpriteMem[s + 2] & 0x03);

				state->SpriteTempMem_HFlipFlag[t] = (state->SpriteMem[s + 2] & 64) ? 1 : 0;
				state->SpriteTempMem_Priority[t] = (state->SpriteMem[s + 2] & 32) ? 1 : 0;
				state->SpriteTempMem_IsSprite0[t] = (spriteNum == 0) ? 1 : 0;

				state->SpriteTempMem_X[t] = state->SpriteMem[s + 3];
			}


		}

		
	}
}