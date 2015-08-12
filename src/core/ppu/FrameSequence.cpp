#include "FrameSequence.h"

namespace PPUCore
{

	void advancePPUByPixels(PPUState *state, int numPixels)
	{
		while (numPixels > 0)
		{
			int pixelsLeftOnScanline = state->scanlineOffset >= 341 ? 0 : (341 - state->scanlineOffset);
			int scanlinePixels; //Number of pixels to advance on same scanline
			bool scanlineBeingCompleted = false;

			if (numPixels >= pixelsLeftOnScanline)
			{
				scanlineBeingCompleted = true;
				scanlinePixels = pixelsLeftOnScanline;
			}
			else
				scanlinePixels = numPixels;

			//Advance as many pixels as possible while remaining on same scanline
			//The following scanline procedures are each responsible for updating the scanline offset, while advancing scanlinePixels
			if (state->scanlineNumber < 20)
				scanline_VBlank_advancePixels(state, scanlinePixels);
			else if (state->scanlineNumber == 20)
				scanline_Dummy_advancePixels(state, scanlinePixels);
			else if (state->scanlineNumber < 261)
				scanline_Visible_advancePixels(state, scanlinePixels);
			else
				scanline_Blank_advancePixels(state, scanlinePixels);

			if (scanlineBeingCompleted)
			{
				state->scanlineNumber++;
				state->scanlineOffset = 0;
			}

			numPixels -= scanlinePixels;
		}
	}


	void advanceToFrameEnd(PPUState *state)
	{
		int totalPixelsLeft = 0;
		if (state->scanlineOffset < 341 )
			totalPixelsLeft += (341 - state->scanlineOffset);  //portion of current scanline left
		if (state->scanlineNumber < 262 )
			totalPixelsLeft += (341 * (261 - state->scanlineNumber)); //based on number of COMPLETE scanlines left
		advancePPUByPixels(state, totalPixelsLeft);
	}


	void scanline_VBlank_advancePixels(PPUState *state, int numPixels)
	{
		state->scanlineOffset += numPixels;
	}


	void scanline_Dummy_advancePixels(PPUState *state, int numPixels)
	{
		if (numPixels <= 0) return;

		int pixelsLeft = numPixels;
		int startingOffset = state->scanlineOffset;
		int endingOffset = startingOffset + numPixels - 1;

		state->scanlineSchedule->executePreRenderRoutines(state, numPixels);  //Increments scanline offset along the way
	}

	void scanline_Visible_advancePixels(PPUState *state, int numPixels)
	{
		if (numPixels <= 0) return;

		int pixelsLeft = numPixels;
		int startingOffset = state->scanlineOffset;
		int endingOffset = startingOffset + numPixels - 1;

		state->scanlineSchedule->executeRoutines(state, numPixels);  //Increments scanline offset along the way
		
		//Draw any completed visible pixels
		if (startingOffset < 256 && state->scanlineOffset > startingOffset)
		{
			int lastVisiblePixelCompleted = 255;
			if (state->scanlineOffset < 256)
				lastVisiblePixelCompleted = (state->scanlineOffset) - 1;

			//Combine the completed BG Pixels with the sprite pixels, filling in the scanline buffer
			Multiplexer::multiplexPixels(state, startingOffset, lastVisiblePixelCompleted);

			//Tell Renderer to render the portion of the scanline buffer filled in to the screen
			state->renderer->drawPixels(state->scanlineNumber, startingOffset, lastVisiblePixelCompleted, state->ScanlineBuffer);
		}
	}

	void scanline_Blank_advancePixels(PPUState *state, int numPixels)
	{
		//No operations to perform
		state->scanlineOffset += numPixels;
	}
}