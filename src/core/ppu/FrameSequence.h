#ifndef PPU_FRAMESEQUENCE_H
#define PPU_FRAMESEQUENCE_H

#include "PPUState.h"
#include "ScanlineRoutines.h"
#include "Multiplexer.h"

namespace PPUCore
{
	void advancePPUByPixels(PPUState *state, int numPixels);
	void advanceToFrameEnd(PPUState *state);

	void scanline_VBlank_advancePixels(PPUState *state, int numPixels);
	void scanline_Dummy_advancePixels(PPUState *state, int numPixels);
	void scanline_Visible_advancePixels(PPUState *state, int numPixels);
	void scanline_Blank_advancePixels(PPUState *state, int numPixels);

}


#endif

