#ifndef PPU_SCANLINE_ROUTINES_H
#define PPU_SCANLINE_ROUTINES_H

#include "PPU.h"
#include "PPUState.h"
#include "DrawSprites.h"
#include "DataFetching.h"
#include "SpriteEvaluation.h"


namespace PPUCore
{
	/**
	* For a Visible Scanline, the following actions occur for each PPUClock/Pixel:
	*
	*  Beggining of Offset 0:  clear the Sprite Scanline buffer then draw the 0-8 sprites contained in sprite temp mem
	*
	*  For Offsets 0-255;
	*      The following sequence of 4 PPU Fetches (takes 8 PPU Clocks) repeats 32 times. Each one of these 32 sequences retrieves data to render 8 upcoming BG Pixels
	*        1) PPU Fetches a name table byte (the fetched value will indicate what pattern's pixel data is fetched in steps 3 and 4)
	*        2) PPU Fetches an attribute table byte
	*        3) PPU Fetches the lower bits of the pattern data
	*        4) PPU Fetches the upper bits of the pattern data
	*
	*        *The BG Scanline buffer is being drawn to during each of these 256 visible pixel drawing offsets
	*
	*  ASSUMPTION:  Due to lack of details on timing of SPRITE EVALUATIONS, all 64 sprite memory entries are assumed to be evaluated
	*               at the beggining of offset 256 (before the Name table byte fetch). This process fills in sprite temp memory with 
	*               the first 8 sprites found to be on the next scanline, by looking at the 64 entries in sprite memory.
	*
	*               Also during Offset 256:  all horizontal BG Scroll Counters reloaded with latched values, and vertical counters incremented
	*                 
	*  For Offsets 256-319:
	*      The following sequence of 4 PPU Fetches repeats 8 times. (retreives the pixel data for the 8 sprites in range of the next scanline)
	*        1) Name table byte fetch (value not used)
	*        2) Name table byte fetch (value not used)
	*        3) PPU Fetches bitplane 0 pattern data for the current sprite 
	*        4) PPU Fetches bitplane 1 pattern data for the current sprite
	*      
	*  For Offsets 320-335:
	*      The following sequence of PPU Fetches repeats 2 times.  This is the same process done during offsets 0-255 but this time
	*      the first two tiles (16 pixels) of BG Pixel data for the next scanline is being fetched (is being fetched early to feed the pipeline of shift registers in prep for next scanline)
	*        1) PPU Fetches a name table byte (the fetched value will indicate what pattern's pixel data is fetched in steps 3 and 4)
	*        2) PPU Fetches an attribute table byte
	*        3) PPU Fetches the lower bits of the pattern data
	*        4) PPU Fetches the upper bits of the pattern data
	*
	*  For Offsets 336-339:
	*      Offset 336,337:   PPU Name table byte fetch (value not used)
	*      Offset 338,339:   PPU Name table byte fetch (value not used)
	*/

	typedef void(*PPUCLOCKROUTINE)(PPUState* p);


	class ScanlineSchedule
	{
	public:
		ScanlineSchedule();
		~ScanlineSchedule();

		void setupSchedule(PPUState* state);

		void executePreRenderRoutines(PPUState* state, int numPixels);

		void executeRoutines(PPUState* state, int numPixels);

		void setBGDrawingMode(bool enableBGDrawing, bool enableBGClipping);
		void setSpriteDrawingMode(bool enableSpriteDrawing, bool enableSpriteCliping);

		void setPPUOn();   //To be called either BG or Sprites has become enabled
		void setPPUOff();  //To be called whenever both BG and Sprites have become disabled

	private:
		PPUCLOCKROUTINE DataFetchingSchedule[341]; //Handles Data Fetching for BG/Sprite Pixel data as well as counter operations
		PPUCLOCKROUTINE BGDrawingSchedule[341];    //Handles whether to draw a BG Pixel from the BG Shift Registers or not (such as when BG is off or clipping in first 8 pixels of BG)
		PPUCLOCKROUTINE SpriteEvalSchedule[341];
		PPUCLOCKROUTINE SpriteDrawingSchedule[341]; //Handles everything for sprite evaluating and drawing.


		static void routine_FetchNTByte(PPUState* state);
		static void routine_FetchATByte(PPUState* state);
		static void routine_FetchPT0Byte(PPUState* state);
		static void routine_FetchPT1Byte(PPUState* state);

		static void routine_offset256(PPUState* state);

		static void transferSpriteFromTempMem(PPUState* state, int spriteNum);
		static void clearSpritePixelData(PPUState* state, int spriteNum);

		static void routine_fetchSprite0_Bitplane0(PPUState* state);
		static void routine_fetchSprite1_Bitplane0(PPUState* state);
		static void routine_fetchSprite2_Bitplane0(PPUState* state);
		static void routine_fetchSprite3_Bitplane0(PPUState* state);
		static void routine_fetchSprite4_Bitplane0(PPUState* state);
		static void routine_fetchSprite5_Bitplane0(PPUState* state);
		static void routine_fetchSprite6_Bitplane0(PPUState* state);
		static void routine_fetchSprite7_Bitplane0(PPUState* state);

		static void routine_fetchSprite0_Bitplane1(PPUState* state);
		static void routine_fetchSprite1_Bitplane1(PPUState* state);
		static void routine_fetchSprite2_Bitplane1(PPUState* state);
		static void routine_fetchSprite3_Bitplane1(PPUState* state);
		static void routine_fetchSprite4_Bitplane1(PPUState* state);
		static void routine_fetchSprite5_Bitplane1(PPUState* state);
		static void routine_fetchSprite6_Bitplane1(PPUState* state);
		static void routine_fetchSprite7_Bitplane1(PPUState* state);

		static void drawBGPixel(PPUState* state); //Called from the routines for offsets 0 - 255 
		static void drawBlankBGPixel(PPUState* state);

		static void clockShiftRegisters(PPUState* state);


		static void evalSprite(PPUState* state);

		static void maskSpritePixel(PPUState* state);

		static void routine_predrawSprites(PPUState* state);
	};


	



}

#endif
