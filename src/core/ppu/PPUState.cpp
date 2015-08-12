#include "PPUState.h"

using namespace PPUCore;

PPUState::PPUState()
{
	//Initially, PPU uses the internal 8KB of CHR RAM
	this->CHRMemPointer = InternalCHRRam;

	//Initially using horizontal mirroring, with the 2KB of internal Name Table Memory
	this->NameTableMemPointers[0] = InternalNameTable0;
	this->NameTableMemPointers[1] = InternalNameTable1;
	this->NameTableMemPointers[2] = InternalNameTable0;
	this->NameTableMemPointers[3] = InternalNameTable1;


	this->lastPPURegWritevalue = 0;

	this->toggle = 0;
	this->VRamAddressIncrement = 0;

	this->Port2007ReadBuffer = 0;

	this->NMIOnVBlank = 0;

	for (int i = 0; i < 32; i++)
		PalMem[i] = 0;

	enableGrayscaleMode = false;

	for (int i = 0; i < 8192; i++)
		InternalCHRRam[i] = 0;

	for (int i = 0; i < 2048; i++)
	{
		InternalNameTable0[i] = 0;
		InternalNameTable1[i] = 0;
	}


	
	this->BGEnabled = false;
	this->BGClipping = false; //By default, render first 8 pixels of BG

	this->BGScrollCountersLatch = 0;
	this->BGScrollFineVerticalLatch = 0;
	this->BGScrollFineHorizontal = 0;
	this->BGScrollCounters = 0;
	this->PAR = 0;
	this->BGAttributeBitsLatch = 0;
	this->BGAttributeBits = 0;
	this->BGBitplane0Latch = 0;
	this->BGBitplane1Latch = 0;
	this->BGShifterBitplane0 = 0;
	this->BGShifterBitplane1 = 0;

	this->BGAttributeBit0Shifter = 0;
	this->BGAttributeBit1Shifter = 0;

	this->scanlineNumber = 0;
	this->scanlineOffset = 0;

	this->VBlankFlag = 0;
	this->Sprite0HitFlag = 0;

	this->SpritesEnabled = false;
	this->SpriteClipping = false;

	this->SpriteSize8x16 = 0;
	this->SpritePatternTable = 0;

	this->SpriteMemAddress = 0;

	for (int i = 0; i < 256; i++)
		SpriteMem[i] = 0;

	this->SpriteOverflowFlag = 0;
	this->SpritesOnScanline = 0;

	this->SpritesOnScanline = 0;
	for (int i = 0; i < 8; i++)
	{
		this->SpriteTempMem_YDelta[i] = 0;
		this->SpriteTempMem_Pattern[i] = 0;
		this->SpriteTempMem_Pal[i] = 0;
		this->SpriteTempMem_Priority[i] = 0;
		this->SpriteTempMem_HFlipFlag[i] = 0;
		this->SpriteTempMem_IsSprite0[i] = 0;
		this->SpriteTempMem_X[i] = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		SpritePixelData_Bitplane0[i] = 0;
		SpritePixelData_Bitplane1[i] = 0;
		SpritePixelData_Pal[i] = 0;
		SpritePixelData_Priority[i] = 0;
		SpritePixelData_IsSprite0[i] = 0;
		SpritePixelData_X[i] = 0;
	}


	for (int i = 0; i < 256; i++)
	{
		this->SpriteScanlineBuffer[i] = 0;
		this->BGScanlineBuffer[i] = 0;
		this->ScanlineBuffer[i] = 0;
	}

}


PPUState::~PPUState()
{
}



void PPUState::setPPUDrawingMode(bool enableBG, bool enableBGClipping, bool enableSprites, bool enableSpriteClipping)
{
	bool BGEnabled_Previous = this->BGEnabled;
	bool SpritesEnabled_Previous = this->SpritesEnabled;

	this->BGEnabled = enableBG;
	this->SpritesEnabled = enableSprites;

	
	this->BGClipping = enableBGClipping;
	this->SpriteClipping = enableSpriteClipping;

	this->scanlineSchedule->setBGDrawingMode(enableBG, enableBGClipping);
	this->scanlineSchedule->setSpriteDrawingMode(enableSprites, enableSpriteClipping);
	
	bool PPUPreviouslyEnabled = (BGEnabled_Previous || SpritesEnabled_Previous);

	if (PPUPreviouslyEnabled && (this->BGEnabled == false) && (this->SpritesEnabled == false))  //PPU has become disabled (was previously enabled)
		this->scanlineSchedule->setPPUOff();

	if (!PPUPreviouslyEnabled && ((this->BGEnabled == true) || (this->SpritesEnabled == true))) //PPU has become enabled (wasn't previously)
		this->scanlineSchedule->setPPUOn();
}


void PPUState::setNTMirroring(NTMIRRORING mirroringType)
{
	this->mirroring = mirroringType;

	if (this->mirroring == NTMIRRORING::HORIZONTAL)
	{
		this->NameTableMemPointers[0] = InternalNameTable0;
		this->NameTableMemPointers[1] = InternalNameTable0;
		this->NameTableMemPointers[2] = InternalNameTable1;
		this->NameTableMemPointers[3] = InternalNameTable1;
	}
	else if (this->mirroring == NTMIRRORING::VERTICAL)
	{
		this->NameTableMemPointers[0] = InternalNameTable0;
		this->NameTableMemPointers[1] = InternalNameTable1;
		this->NameTableMemPointers[2] = InternalNameTable0;
		this->NameTableMemPointers[3] = InternalNameTable1;
	}
	else if (this->mirroring == NTMIRRORING::FOURSCREEN)
	{
		this->NameTableMemPointers[0] = InternalNameTable0;
		this->NameTableMemPointers[1] = InternalNameTable1;
		this->NameTableMemPointers[2] = InternalNameTable2;
		this->NameTableMemPointers[3] = InternalNameTable3;
	}
	else if (this->mirroring == NTMIRRORING::ONESCREEN_LOWER)
	{
		this->NameTableMemPointers[0] = InternalNameTable0;
		this->NameTableMemPointers[1] = InternalNameTable0;
		this->NameTableMemPointers[2] = InternalNameTable0;
		this->NameTableMemPointers[3] = InternalNameTable0;
	}
	else if (this->mirroring == NTMIRRORING::ONESCREEN_UPPER)
	{
		this->NameTableMemPointers[0] = InternalNameTable1;
		this->NameTableMemPointers[1] = InternalNameTable1;
		this->NameTableMemPointers[2] = InternalNameTable1;
		this->NameTableMemPointers[3] = InternalNameTable1;
	}
}