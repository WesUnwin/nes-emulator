#ifndef PPU_STATE_H
#define PPU_STATE_H

#include "Renderer.h"


namespace PPUCore
{
	class ScanlineSchedule;


	class PPUState
	{
	public:
		PPUState();
		~PPUState();

		enum NTMIRRORING { HORIZONTAL, VERTICAL, FOURSCREEN, ONESCREEN_LOWER, ONESCREEN_UPPER };

		void setNTMirroring(NTMIRRORING mirroringType);

		void setPPUDrawingMode(bool enableBG, bool enableBGClipping, bool enableSprites, bool enableSpriteClipping);


		unsigned char lastPPURegWritevalue;

		Renderer* renderer;

		int toggle;
		int VRamAddressIncrement;

		unsigned char Port2007ReadBuffer;


		
		NTMIRRORING mirroring;

		int NMIOnVBlank;

		//Pallette Memory (programmable 32 bytes of pal mem inside PPU)
		unsigned char PalMem[32];

		bool enableGrayscaleMode;

		//Internal 8KB of CHR Ram
		unsigned char *CHRMemPointer; //Pointer to 8KB of char data 

		unsigned char InternalCHRRam[8192];  //8KB CHR Ram in NES (can be pointed to by CHRMemPointer)

		unsigned char* NameTableMemPointers[4];  //Array of 4 pointers, each pointing to the 1KB of memory mapped into each of $2000, $2400, $2800, $2C00 

		unsigned char InternalNameTable0[2048];
		unsigned char InternalNameTable1[2048];
		unsigned char InternalNameTable2[2048]; //used for 4-Screen mirroring
		unsigned char InternalNameTable3[2048]; //used for 4-Screen mirroring

		//BG Settings
		bool BGEnabled;
		bool BGClipping;

		//Sprite Settings
		bool SpritesEnabled;

		int SpriteSize8x16;
		int SpritePatternTable;
		bool SpriteClipping;

		//Sprite Registers/Mem
		unsigned char SpriteMemAddress;
		unsigned char SpriteMem[256];

		int SpriteOverflowFlag;
		int SpritesOnScanline;

		unsigned char SpriteTempMem_YDelta[8];
		unsigned char SpriteTempMem_Pattern[8];
		unsigned char SpriteTempMem_X[8];
		unsigned char SpriteTempMem_IsSprite0[8];
		unsigned char SpriteTempMem_Priority[8];
		unsigned char SpriteTempMem_HFlipFlag[8];
		unsigned char SpriteTempMem_Pal[8];

		unsigned char SpritePixelData_Bitplane0[8];
		unsigned char SpritePixelData_Bitplane1[8];
		unsigned char SpritePixelData_Pal[8];
		unsigned char SpritePixelData_Priority[8];
		unsigned char SpritePixelData_IsSprite0[8];
		unsigned char SpritePixelData_X[8];
			
		//BG Scroll Counters & Latches
		unsigned int BGScrollCountersLatch;     //VHYYYYYXXXXX
		unsigned int BGScrollFineVerticalLatch; //         yyy
		unsigned int BGScrollFineHorizontal;    //xxx
		unsigned int BGScrollCounters;		    //VHYYYYYXXXXX

		unsigned int PAR;   //SppppppppByyy

		unsigned int BGAttributeBitsLatch;
		unsigned int BGAttributeBits;

		unsigned char BGBitplane0Latch;
		unsigned char BGBitplane1Latch;
		unsigned short int BGShifterBitplane0;
		unsigned short int BGShifterBitplane1;

		unsigned char BGAttributeBit0Shifter;
		unsigned char BGAttributeBit1Shifter;

		/**
		*
		*  Scanlines    0-19:   VBlank
		*  Scanline       20:   Dummy Scanline
		*  Scanlines  21-260:   Visible Scanlines (x240)
		*  Scanline      261:   Blank Scanline
		*/
		int scanlineNumber;

		/**
		*  For Visible Scanlines:
		*  Offsets     0-255:   Visible pixels of scanline, from left to right across screen
		*  Offsets   256-340:   HBlank
		*/
		int scanlineOffset;

		int VBlankFlag;
		int Sprite0HitFlag;

		/**
		*
		*  Each Byte has the following format:
		*    Bit 7:  Sprite 0 Flag  (=1 if this is a pixel of sprite 0, elsewise is 0)
		*    Bit 6:  Priority (0:OverBG 1:UnderBG)
		*
		*    Bit 3:  Upper of 2 attribute bits  (select which sub-palette of sprite palette)
		*    Bit 2:  Lower of 2 attribute bits  (select which sub-palette of sprite palette)
		*
		*    Bit 1:  Upper of 2 bits indicating pixel colour  (from bitplane 1)
		*    Bit 0:  Lower of 2 bits indicating pixel colour  (from bitplane 0)
		*
		*/
		unsigned char SpriteScanlineBuffer[300];

		/*
		*  Eacy Byte has the following format:
		*
		*    Bit 3:  Upper of 2 attribute bits  (select which sub-palette of BG palette)
		*    Bit 2:  Lower of 2 attribute bits  (select which sub-palette of BG palette)
		*
		*    Bit 1:  Upper of 2 bits indicating pixel colour  (from bitplane 1)
		*    Bit 0:  Lower of 2 bits indicating pixel colour  (from bitplane 0)
		*/
		unsigned char BGScanlineBuffer[256];

		/**
		*  Each Byte is an index into the master palette indicating what colour to render
		*  for each pixel of the 256 visible pixels off the current scanline.
		*  Each Bytes is the 6 bit output of the multiplexer, with inputs to the multiplexer being the 4-bit value
		*  from the same pixel of the BGScanlineBuffer and the 5 bit input of the same pixel from the SpriteScanlineBuffer
		*/
		unsigned char ScanlineBuffer[256];

		ScanlineSchedule* scanlineSchedule;
	};




}


#include "ScanlineRoutines.h"

#endif