#include "ScanlineRoutines.h"

namespace PPUCore
{
	ScanlineSchedule::ScanlineSchedule()
	{

	}

	ScanlineSchedule::~ScanlineSchedule()
	{

	}

	/**
	* Based on PPU State / Settings, setup the arrays of function pointers to schedule the corresponding 
	* operation for each pixel/offset.
	*/
	void ScanlineSchedule::setupSchedule(PPUState* state)
	{
		if (state->BGEnabled || state->SpritesEnabled)
			setPPUOn();
		else
			setPPUOff();


		//Setup BG Drawing Routines
		for (int i = 0; i < 341; i++)
			BGDrawingSchedule[i] = 0;  //Clear All

		for (int i = 0; i < 256; i++)
			BGDrawingSchedule[i] = drawBGPixel;

		for (int i = 320; i < 336; i++)
			BGDrawingSchedule[i] = clockShiftRegisters;  //the shift registers need to be moving when prefetching first 16 pixels of BG

		setBGDrawingMode(state->BGEnabled, state->BGClipping);

		//Setup Sprite Evaluation Routines
		for (int i = 0; i < 341; i++)
			SpriteEvalSchedule[i] = 0;

		for (int i = 0; i < 64; i++)
			SpriteEvalSchedule[(i*4) + 3] = evalSprite;

		SpriteEvalSchedule[0] = routine_predrawSprites;


		//Sprite Drawing Routines
		for (int i = 0; i < 341; i++)
			SpriteDrawingSchedule[i] = 0;

		setSpriteDrawingMode(state->SpritesEnabled, state->SpriteClipping);
	}


	void ScanlineSchedule::executePreRenderRoutines(PPUState* state, int numPixels)
	{
		if (numPixels <= 0) return;

		int startingOffset = state->scanlineOffset;
		int endingOffset = startingOffset + numPixels - 1;
		for (int i = startingOffset; i <= endingOffset; i++)
		{
			if (i == 0)
			{
				state->VBlankFlag = 0;
				state->Sprite0HitFlag = 0;
				state->SpriteOverflowFlag = 0;
			}
			
			//Do Data Fetch
			PPUCLOCKROUTINE dataFetchRoutine = DataFetchingSchedule[i];
			if (dataFetchRoutine != 0) dataFetchRoutine(state);

			if ((i >= 279) && (i <= 303))
			{
				//For each of these clocks Vertical Scroll Counters are forced equal to their corresponding scroll latches
				if (state->BGEnabled || state->SpritesEnabled)
					ScrollCounterOps::loadLatchedVerticalCounters(state);
			}
			
			if ((i >= 320) && (i < 336))
				clockShiftRegisters(state);  //the shift registers need to be moving when prefetching first 16 pixels of BG

			//No BG or Sprite Drawing
			//No Secondary OAM Clearing, No Sprite Evaluations

			state->scanlineOffset++;
		}
	}


	void ScanlineSchedule::executeRoutines(PPUState* state, int numPixels)
	{
		if (numPixels <= 0) return;

		int startingOffset = state->scanlineOffset;
		int endingOffset = startingOffset + numPixels - 1;
		for (int i = startingOffset; i <= endingOffset; i++)
		{
		
			//Do Data Fetch
			PPUCLOCKROUTINE dataFetchRoutine = DataFetchingSchedule[i];
			if (dataFetchRoutine != 0) dataFetchRoutine(state);

			//Do BG Drawing
			PPUCLOCKROUTINE bgDrawingRoutine = BGDrawingSchedule[i];
			if (bgDrawingRoutine != 0) bgDrawingRoutine(state);

			//Do Sprite Evaluation
			PPUCLOCKROUTINE spriteEvalRoutine = SpriteEvalSchedule[i];
			if (spriteEvalRoutine != 0) spriteEvalRoutine(state);

			//Do Sprite Drawing
			PPUCLOCKROUTINE spriteDrawRoutine = SpriteDrawingSchedule[i];
			if (spriteDrawRoutine != 0) spriteDrawRoutine(state);


			state->scanlineOffset++;
		}
	}



	void ScanlineSchedule::setBGDrawingMode(bool enableBGDrawing, bool enableBGClipping)
	{
		if (enableBGDrawing)
		{
			for (int i = 0; i < 256; i++)
				BGDrawingSchedule[i] = drawBGPixel;

			if (enableBGClipping)
			{
				for (int i = 0; i < 8; i++)
					BGDrawingSchedule[i] = drawBlankBGPixel;
			}
			else
			{
				for (int i = 0; i < 8; i++)
					BGDrawingSchedule[i] = drawBGPixel;
			}
		}
		else
		{
			for (int i = 0; i < 256; i++)
				BGDrawingSchedule[i] = drawBlankBGPixel;
		}
	}


	void ScanlineSchedule::setSpriteDrawingMode(bool enableSpriteDrawing, bool enableSpriteCliping)
	{
		if (enableSpriteDrawing)
		{
			for (int i = 0; i < 256; i++)
				SpriteDrawingSchedule[i] = 0;  //0 allows sprite to be drawn as predrawn to sprite buffer

			if (enableSpriteCliping)
			{
				for (int i = 0; i < 8; i++)
					SpriteDrawingSchedule[i] = maskSpritePixel;
			}
			else
			{
				for (int i = 0; i < 8; i++)
					SpriteDrawingSchedule[i] = 0;
			}
		}
		else
		{
			for (int i = 0; i < 256; i++)
				SpriteDrawingSchedule[i] = maskSpritePixel;
		}


	}


	void ScanlineSchedule::setPPUOff()
	{
		//Turn off data fetching, and counter incrementing/loading

		for (int i = 0; i < 341; i++)
			DataFetchingSchedule[i] = 0;  //Clear All
	}


	void ScanlineSchedule::setPPUOn()
	{
		//Enable all Data Fetching, and counter incrementing/loading

		for (int i = 0; i < 341; i++)
			DataFetchingSchedule[i] = 0;  //Clear All

		//BG Pixel Fetching Routines
		int offset = 0;
		for (int i = 0; i < 32; i++)
		{
			DataFetchingSchedule[offset] = routine_FetchNTByte;
			offset += 2;
			DataFetchingSchedule[offset] = routine_FetchATByte;
			offset += 2;
			DataFetchingSchedule[offset] = routine_FetchPT0Byte;
			offset += 2;
			DataFetchingSchedule[offset] = routine_FetchPT1Byte;
			offset += 2;
		}

		DataFetchingSchedule[256] = routine_offset256;


		//Routines for fetching the sprite pixel data for the 0-8 sprites in temp memory
		DataFetchingSchedule[260] = routine_fetchSprite0_Bitplane0;
		DataFetchingSchedule[262] = routine_fetchSprite0_Bitplane1;

		DataFetchingSchedule[268] = routine_fetchSprite1_Bitplane0;
		DataFetchingSchedule[270] = routine_fetchSprite1_Bitplane1;

		DataFetchingSchedule[276] = routine_fetchSprite2_Bitplane0;
		DataFetchingSchedule[278] = routine_fetchSprite2_Bitplane1;

		DataFetchingSchedule[284] = routine_fetchSprite3_Bitplane0;
		DataFetchingSchedule[286] = routine_fetchSprite3_Bitplane1;

		DataFetchingSchedule[292] = routine_fetchSprite4_Bitplane0;
		DataFetchingSchedule[294] = routine_fetchSprite4_Bitplane1;

		DataFetchingSchedule[300] = routine_fetchSprite5_Bitplane0;
		DataFetchingSchedule[302] = routine_fetchSprite5_Bitplane1;

		DataFetchingSchedule[308] = routine_fetchSprite6_Bitplane0;
		DataFetchingSchedule[310] = routine_fetchSprite6_Bitplane1;

		DataFetchingSchedule[316] = routine_fetchSprite7_Bitplane0;
		DataFetchingSchedule[318] = routine_fetchSprite7_Bitplane1;


		//Pre-fetching for first 16 pixels of BG Data
		DataFetchingSchedule[320] = routine_FetchNTByte;
		DataFetchingSchedule[322] = routine_FetchATByte;
		DataFetchingSchedule[324] = routine_FetchPT0Byte;
		DataFetchingSchedule[326] = routine_FetchPT1Byte;

		DataFetchingSchedule[328] = routine_FetchNTByte;
		DataFetchingSchedule[330] = routine_FetchATByte;
		DataFetchingSchedule[332] = routine_FetchPT0Byte;
		DataFetchingSchedule[334] = routine_FetchPT1Byte;
	}


	void ScanlineSchedule::routine_FetchNTByte(PPUState* state)
	{
		DataFetching::fetchNTByte(state);
	}

	void ScanlineSchedule::routine_FetchATByte(PPUState* state)
	{
		DataFetching::fetchATByte(state);
	}

	void ScanlineSchedule::routine_FetchPT0Byte(PPUState* state)
	{
		DataFetching::fetchPTByte_BitPlane0(state);
	}

	void ScanlineSchedule::routine_FetchPT1Byte(PPUState* state)
	{
		DataFetching::fetchPTByte_BitPlane1(state);
		ScrollCounterOps::incrementHorizontalScrollCounters(state);
	}
	



	void ScanlineSchedule::routine_offset256(PPUState* state)
	{
		ScrollCounterOps::incrementVerticalCounters(state);
		ScrollCounterOps::loadLatchedHorizontalCounters(state);		
	}



	void ScanlineSchedule::transferSpriteFromTempMem(PPUState* state, int spriteNum)
	{
		state->SpritePixelData_Pal[spriteNum] = state->SpriteTempMem_Pal[spriteNum];
		state->SpritePixelData_Priority[spriteNum] = state->SpriteTempMem_Priority[spriteNum];
		state->SpritePixelData_IsSprite0[spriteNum] = state->SpriteTempMem_IsSprite0[spriteNum];
		state->SpritePixelData_X[spriteNum] = state->SpriteTempMem_X[spriteNum];
	}

	void ScanlineSchedule::clearSpritePixelData(PPUState* state, int spriteNum)
	{
		state->SpritePixelData_Bitplane0[spriteNum] = 0;
		state->SpritePixelData_Bitplane1[spriteNum] = 0;
		state->SpritePixelData_Pal[spriteNum] = 0;
		state->SpritePixelData_Priority[spriteNum] = 0;
		state->SpritePixelData_IsSprite0[spriteNum] = 0;
		state->SpritePixelData_X[spriteNum] = 0;
	}

	//Routines for fetching pattern data (bitplane 0) for temp sprites
	void ScanlineSchedule::routine_fetchSprite0_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 1)
		{
			DataFetching::fetchSpritePatternData(state, 0, 0);
			transferSpriteFromTempMem(state, 0);
		}
		else
			clearSpritePixelData(state, 0);
	}

	void ScanlineSchedule::routine_fetchSprite1_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 2)
		{
			DataFetching::fetchSpritePatternData(state, 1, 0);
			transferSpriteFromTempMem(state, 1);
		}
		else
			clearSpritePixelData(state, 1);
	}

	void ScanlineSchedule::routine_fetchSprite2_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 3)
		{
			DataFetching::fetchSpritePatternData(state, 2, 0);
			transferSpriteFromTempMem(state, 2);
		}
		else
			clearSpritePixelData(state, 2);
	}

	void ScanlineSchedule::routine_fetchSprite3_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 4)
		{
			DataFetching::fetchSpritePatternData(state, 3, 0);
			transferSpriteFromTempMem(state, 3);
		}
		else
			clearSpritePixelData(state, 3);
	}

	void ScanlineSchedule::routine_fetchSprite4_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 5)
		{
			DataFetching::fetchSpritePatternData(state, 4, 0);
			transferSpriteFromTempMem(state, 4);
		}
		else
			clearSpritePixelData(state, 4);
	}

	void ScanlineSchedule::routine_fetchSprite5_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 6)
		{
			DataFetching::fetchSpritePatternData(state, 5, 0);
			transferSpriteFromTempMem(state, 5);
		}
		else
			clearSpritePixelData(state, 5);
	}

	void ScanlineSchedule::routine_fetchSprite6_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 7)
		{
			DataFetching::fetchSpritePatternData(state, 6, 0);
			transferSpriteFromTempMem(state, 6);
		}
		else
			clearSpritePixelData(state, 6);
	}

	void ScanlineSchedule::routine_fetchSprite7_Bitplane0(PPUState* state)
	{
		if (state->SpritesOnScanline >= 8)
		{
			DataFetching::fetchSpritePatternData(state, 7, 0);
			transferSpriteFromTempMem(state, 7);
		}
		else
			clearSpritePixelData(state, 7);
	}




	//Routines for fetching pattern data (bitplane 1) for temp sprites
	void ScanlineSchedule::routine_fetchSprite0_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 1) DataFetching::fetchSpritePatternData(state, 0, 1);
	}

	void ScanlineSchedule::routine_fetchSprite1_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 2) DataFetching::fetchSpritePatternData(state, 1, 1);
	}

	void ScanlineSchedule::routine_fetchSprite2_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 3) DataFetching::fetchSpritePatternData(state, 2, 1);
	}

	void ScanlineSchedule::routine_fetchSprite3_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 4) DataFetching::fetchSpritePatternData(state, 3, 1);
	}

	void ScanlineSchedule::routine_fetchSprite4_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 5) DataFetching::fetchSpritePatternData(state, 4, 1);
	}

	void ScanlineSchedule::routine_fetchSprite5_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 6) DataFetching::fetchSpritePatternData(state, 5, 1);
	}

	void ScanlineSchedule::routine_fetchSprite6_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 7) DataFetching::fetchSpritePatternData(state, 6, 1);
	}

	void ScanlineSchedule::routine_fetchSprite7_Bitplane1(PPUState* state)
	{
		if (state->SpritesOnScanline >= 8) DataFetching::fetchSpritePatternData(state, 7, 1);
	}




	void ScanlineSchedule::drawBGPixel(PPUState* state)
	{
		unsigned char bgPixel = 0;

		int bitSelect = (1 << state->BGScrollFineHorizontal);  //based on fine horizontal scroll

		//Get BG Pixel output from shift registers
		if (state->BGShifterBitplane0 & bitSelect) bgPixel |= 1;
		if (state->BGShifterBitplane1 & bitSelect) bgPixel |= 2;

		//Apply the 2 Attribute Bits to the BG Pixel
		if (state->BGAttributeBit0Shifter & bitSelect) bgPixel |= 4;
		if (state->BGAttributeBit1Shifter & bitSelect) bgPixel |= 8;

		state->BGScanlineBuffer[state->scanlineOffset] = bgPixel;
		clockShiftRegisters(state);
	}
	
	void ScanlineSchedule::drawBlankBGPixel(PPUState* state)
	{
		state->BGScanlineBuffer[state->scanlineOffset] = 0;
		clockShiftRegisters(state);
	}


	/**
	* Clocks all the PPU's shift registers
	*/
	void ScanlineSchedule::clockShiftRegisters(PPUState* state)
	{
		//Shift BG Pixel Shift Registers after every PPUClock
		state->BGShifterBitplane0 = state->BGShifterBitplane0 >> 1;
		state->BGShifterBitplane1 = state->BGShifterBitplane1 >> 1;
		state->BGAttributeBit0Shifter = state->BGAttributeBit0Shifter >> 1;
		state->BGAttributeBit1Shifter = state->BGAttributeBit1Shifter >> 1;
		if (state->BGAttributeBits & 0x1) state->BGAttributeBit0Shifter |= 128;
		if (state->BGAttributeBits & 0x2) state->BGAttributeBit1Shifter |= 128;
	}


	void ScanlineSchedule::routine_predrawSprites(PPUState* state)
	{
		drawSprites(state); //Quickly draws all sprites to a buffer (rather then each pixel operating shift registers)
							//The pixels in the buffer are then selectively used (or masked/set to zero) as sprite display is enabled/disabled/clipped
		state->SpritesOnScanline = 0;
	}

	void ScanlineSchedule::evalSprite(PPUState* state)
	{
		//Based on scanline offset, evaluated next of the 64 sprites to see if in range (if sprite is in range its added to sprite temp mem)
		int spriteNum = (state->scanlineOffset >> 2);
		
		PPUCore::SpriteEvaluations::doSpriteEvaluation(state, spriteNum);
	}

	void ScanlineSchedule::maskSpritePixel(PPUState* state)
	{
		state->SpriteScanlineBuffer[state->scanlineOffset] = 0;  //Don't draw current sprite pixel (elsewise pixel in sprite buffer is left as is)
	}
}