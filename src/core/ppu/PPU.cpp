#include "PPU.h"

namespace PPUCore
{
	PPU::PPU(Renderer* renderer)
	{
		this->state = new PPUState();
		this->state->renderer = renderer;
		this->state->scanlineSchedule = new ScanlineSchedule();
		this->state->scanlineSchedule->setupSchedule(this->state);
		
		newFrame();
	}


	PPU::~PPU()
	{
		delete state;
	}


	bool PPU::newFrame()
	{
		this->state->scanlineNumber = 0;
		this->state->scanlineOffset = 0;

		this->state->renderer->newFrame();

		this->state->VBlankFlag = 1;
		if (this->state->NMIOnVBlank != 0)
			return true;
		else
			return false;
	}


	void PPU::advanceByPPUClocks(int ppuClocks)
	{
		advancePPUByPixels(this->state, ppuClocks);
	}


	void PPU::finishFrame()
	{
		advanceToFrameEnd(this->state);
	}


	void PPU::requestRedraw()
	{
		this->state->renderer->requestRedraw();
	}


	int PPU::getPPUClocksIntoFrame()
	{
		return (this->state->scanlineNumber * 341) + this->state->scanlineOffset;
	}


	unsigned char PPU::readPort(int PortNum)
	{
		unsigned char result = 0;
		bool palRead = false;
		unsigned char NTATByteUnderneath = 0;

		switch (PortNum)
		{
		case 2:
			result = (state->lastPPURegWritevalue & 0x1F);
			if (state->VBlankFlag != 0) result |= 0x80;
			if (state->Sprite0HitFlag != 0) result |= 64;
			if (state->SpriteOverflowFlag != 0) result |= 32;
			state->VBlankFlag = 0;
			state->toggle = 0;
			return result;
		case 4:
			result = state->SpriteMem[state->SpriteMemAddress];  //OAM Address does not increment on reads
			return result;
		case 7:
			result = state->Port2007ReadBuffer;
			state->Port2007ReadBuffer = PPUCore::AddressSpace::readBy2007(state, palRead, NTATByteUnderneath);
			
			incrementVRAMAddress(state); //increments VRAM address appropriately

			//If PAL was read return the read value directly (also put value in read buffer for next read)
			if (palRead)
			{
				result = state->Port2007ReadBuffer;
				state->Port2007ReadBuffer = NTATByteUnderneath;
			}

			return result;

		default:
			return 0x80; //Value to be returned on reading unreadable port
		}
	}


	void PPU::writeToPort(int PortNum, unsigned char value)
	{
		bool enableBG;
		bool enableBGClipping;
		bool enableSprites;
		bool enableSpriteClipping;

		state->lastPPURegWritevalue = value;

		switch (PortNum)
		{
		case 0:		//PPU Control Register 0
			PPUCore::ScrollCounterOps::port0Write(state, value);  //Bits 1 & 0 Set VH Name table select bits

			state->VRamAddressIncrement = (value & 4) ? 1 : 0;  //Bit 2 Set PPU Address Increment (0:Increment by 1  1:Increment by 32)

			state->SpritePatternTable = (value & 8) ? 1 : 0;  //Bit 3 Sprite Pattern Table

			if (value & 16) state->PAR |= 0x1000;  //Bit 4 set BG Pattern Table
			else state->PAR &= 0xFFFFEFFF;

			state->SpriteSize8x16 = (value & 32) ? 1 : 0;  //Bit 5:  Sprite Size (0:8x8  1:8x16)

			//Bit 6:  Sets EXT Bus direction  (on an NES this 4 pin bus on the PPU used for master/slave PPU communication is unused)

			state->NMIOnVBlank = (value & 128) ? 1 : 0;  //Bit 7 enables NMIs on VBlank

			break;
		case 1:		//PPU Control Register 1

			state->enableGrayscaleMode = (value & 1) ? true : false; //Bit 0:   GrayScale Mode

			enableBGClipping = value & 2 ? false : true;		//Bit 1:   BG Clipping    (Render when 1,  enable clipping when 0)
			enableSpriteClipping = value & 4 ? false : true;	//Bit 2:   Sprite Clipping 
			enableBG = value & 8 ? true : false;				//Bit 3:   Enable BG Display
			enableSprites = value & 16 ? true : false;			//Bit 4:   Enable Sprite Display

			state->setPPUDrawingMode( enableBG, enableBGClipping, enableSprites, enableSpriteClipping);

			//Bit 5:   R
			//Bit 6:   G
			//Bit 7:   B
			break;
		case 2:		//PPU Status Register
			//Writing to has no effect
			break;
		case 3:		//PPU Sprite Memory Address
			state->SpriteMemAddress = value;
			break;
		case 4:		//PPU Sprite Memory Data
			state->SpriteMem[state->SpriteMemAddress] = value;
			state->SpriteMemAddress++;  //Wraps around from 255 to 0
			break;
		case 5:		//PPU Scroll Register
			if (state->toggle == 0)
			{
				PPUCore::ScrollCounterOps::port5Write_0(state, value);
				state->toggle = 1;
			}
			else
			{
				PPUCore::ScrollCounterOps::port5Write_1(state, value);
				state->toggle = 0;
			}
			break;
		case 6:		//PPU VRam Address
			if (state->toggle == 0)
			{
				PPUCore::ScrollCounterOps::port6Write_0(state, value);
				state->toggle = 1;
			}
			else
			{
				PPUCore::ScrollCounterOps::port6Write_1(state, value);
				state->toggle = 0;
			}
			break;
		case 7:		//PPU VRam Data
			PPUCore::AddressSpace::writeBy2007(state, value);
			incrementVRAMAddress(state);
			break;
		}
	}


	void PPU::incrementVRAMAddress(PPUState* state)
	{
		//Increment VRam Address
		if (state->scanlineNumber >= 20 && state->scanlineNumber != 261 && (state->BGEnabled || state->SpritesEnabled))
		{
			//$2007 access while actively rendering!
			PPUCore::ScrollCounterOps::incrementScrollCountersBy1(state);
			PPUCore::ScrollCounterOps::incrementVerticalCounters(state);
		}
		else
		{
			//$2007 access during VBlank or when PPU is off
			if (state->VRamAddressIncrement == 0)
				PPUCore::ScrollCounterOps::incrementScrollCountersBy1(state);
			else
				PPUCore::ScrollCounterOps::incrementScrollCountersBy32(state);
		}
	}


	void PPU::setCHRMemPointer(unsigned char* chrMem)
	{
		if (chrMem == 0)
		{
			//Use Internal CHR-Ram
			this->state->CHRMemPointer = this->state->InternalCHRRam;
		}
		else
		{
			//Set CHR Mem Pointer to refer to some externally allocated CHR ROM (from Cart)
			this->state->CHRMemPointer = chrMem;
		}
	}


	void PPU::setNTMirroring(PPUState::NTMIRRORING mirroringType)
	{
		this->state->setNTMirroring(mirroringType);
	}

	PPUState* PPU::getState()
	{
		return this->state;
	}
}