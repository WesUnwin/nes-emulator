#ifndef PPU_MULTIPLEXER_H
#define PPU_MULTIPLEXER_H

#include "PPUState.h"

namespace PPUCore
{
	namespace Multiplexer
	{
		//Combine Sprite and BG Pixels selectively, based on logic of PPU Multiplexer
		void multiplexPixels(PPUState* state, int startingOffset, int endingOffset);
	}
}

#endif