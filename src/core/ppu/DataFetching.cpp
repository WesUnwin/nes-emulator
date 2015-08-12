#include "DataFetching.h"


namespace PPUCore
{
	namespace DataFetching
	{
		void fetchNTByte(PPUState *state)
		{
			//Load BG Pattern Shifters With Pattern Latch
			state->BGShifterBitplane0 &= 0x00FF;
			state->BGShifterBitplane0 |= (((unsigned int)state->BGBitplane0Latch) << 8);
			state->BGShifterBitplane1 &= 0x00FF;
			state->BGShifterBitplane1 |= (((unsigned int)state->BGBitplane1Latch) << 8);

			//Load Attribute Bits with Latched
			state->BGAttributeBits = state->BGAttributeBitsLatch;

			//Based on ScrollCounters, will fetch a Name table byte which indicates the pattern to be rendered in the next 8 pixels of the pipeline
			unsigned short int address = (state->BGScrollCounters & 0x0FFF);
			address |= 0x2000;  //DC address lines forced to 10
			unsigned char PatternNum = PPUCore::AddressSpace::readInternal(state, address);

			state->PAR &= 0x100F;
			state->PAR |= (PatternNum << 4);
		}

		void fetchATByte(PPUState *state)
		{
			unsigned short int address = 0x23C0; //bits D,C of address are forced to 1,0, and bits 9,8,7,6 of address are forced to 1
			address |= ((state->BGScrollCounters & 0x1C) >> 2); //upper 3 bits of Horizontal Tile Scroll become bits 2,1,0 of address bus
			address |= ((state->BGScrollCounters & 0x380) >> 4); //upper 3 bits of vertical tile become bits 5,4,3 of address

			address |= (state->BGScrollCounters & 0xC00); //VH nametable bits become bits B,A of address
						
			unsigned char attributeByte = PPUCore::AddressSpace::readInternal(state, address);

			//Now, read byte is shifted to the right and then the lowest 2 bits are read off (only reading 2 of the 8 bits in the byte read)
			//Shift amount: c,b,a  comes from 2nd lowest bit of vertical tile scroll, 2nd lowest bit of horizontal tile scroll, 0

			int numberOfBitsToShiftRight = 0;
			if (state->BGScrollCounters & 2) numberOfBitsToShiftRight |= 2;
			if (state->BGScrollCounters & 0x40) numberOfBitsToShiftRight |= 4;

			state->BGAttributeBitsLatch = ((attributeByte >> numberOfBitsToShiftRight) & 0x03);
		}

		void fetchPTByte_BitPlane0(PPUState *state)
		{
			//Based on PatterNumber read from NTByte fetch, fetch the row of 8 pixels to be rendered on this scanline
			//Fetch just the byte containing the lower of the 2 bits per pixel, for each of the 8 pixels

			unsigned short int address = (state->PAR & 0x1FF7);
			unsigned char lowerBitsPatternByte = PPUCore::AddressSpace::readInternal(state, address);

			state->BGBitplane0Latch = PPUCore::DataFetching::hFlipByteLookupTable[lowerBitsPatternByte];
		}

		void fetchPTByte_BitPlane1(PPUState *state)
		{
			unsigned short int address = (state->PAR & 0x1FF7) | 8;  //Set the bit which will grab the upper bits byte (8 bytes later than where lower byte is)
			unsigned char upperBitsPatternByte = PPUCore::AddressSpace::readInternal(state, address);

			state->BGBitplane1Latch = PPUCore::DataFetching::hFlipByteLookupTable[upperBitsPatternByte];
		}

		void fetchSpritePatternData(PPUState *state, int spriteTempMemIndex, int bitplane)
		{
			unsigned short int address = 0;
			if (state->SpriteSize8x16 == 0)
			{
				//8x8 Sprites
				address = (state->SpriteTempMem_Pattern[spriteTempMemIndex]) << 4;
				if (state->SpritePatternTable != 0) address |= 0x1000;
				if (bitplane != 0) address |= 0x8;
				address |= (state->SpriteTempMem_YDelta[spriteTempMemIndex] & 0x7);
			}
			else
			{
				//8x16 Sprites
				unsigned char pattern = state->SpriteTempMem_Pattern[spriteTempMemIndex];

				if (pattern & 0x01) address += 0x1000;  //Odd pattern numbers result in the two sprite tiles coming from the 2nd pattern table
				address += ((pattern & 0xFE) >> 1) * 32; //upper bits indicate which pair of 8x8 patterns (side by side in memory) to use

				if (state->SpriteTempMem_YDelta[spriteTempMemIndex] >= 8)
					address += 16; //If YDelta is a row from the bottom half of the 8x16 sprite, get the row from 1 pattern later

				if (bitplane != 0) address += 8; //if fetching the

				address += (state->SpriteTempMem_YDelta[spriteTempMemIndex] & 0x07); //get correct row within the pattern
			}
			
			//Read Byte of Pattern Data
			unsigned char data = PPUCore::AddressSpace::readInternal(state, address);

			//Apply H-Flip if applicable
			if (state->SpriteTempMem_HFlipFlag[spriteTempMemIndex] == 0) data = hFlipByteLookupTable[data]; //Important Note: data will need to be hFlipped normally due to direction of shift registers, if sprite is to be flipped leave data unflipped

			if(bitplane == 0 )
				state->SpritePixelData_Bitplane0[spriteTempMemIndex] = data;
			else
				state->SpritePixelData_Bitplane1[spriteTempMemIndex] = data;
		}
	}
}