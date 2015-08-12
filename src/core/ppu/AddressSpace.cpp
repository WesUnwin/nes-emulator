#include "AddressSpace.h"


namespace PPUCore
{
	namespace AddressSpace
	{

		unsigned char readInternal(PPUState *state, unsigned short int PPUAddress)
		{
			if (PPUAddress < 0x2000)
			{
				//A Pattern Table Memory Read
				return state->CHRMemPointer[PPUAddress];
			}
			else
			{
				//A Name Table Memory Read
				unsigned short int offset = PPUAddress & 0x0FFF;  //Takes care of mirroring of 2000-2FFF which is also seen at 3000-3FFF
				
				unsigned char *NameTableMemPointer = (unsigned char*) (state->NameTableMemPointers[offset >> 10]);

				return NameTableMemPointer[offset & 0x3FF];
			}
		}

		unsigned char readBy2007(PPUState *state, bool& wasPalRead, unsigned char& underlyingNTByte)
		{
			unsigned short int PPUAddress = state->BGScrollCounters;

			unsigned short int fv = (state->PAR & 0x3);
			PPUAddress |= (fv << 12);

			if (PPUAddress >= 0x03F00)
			{
				wasPalRead = true;
				underlyingNTByte = readInternal(state, PPUAddress);

				unsigned short int offset = PPUAddress & 0x1F;  //Takes care of mirroring of pal mem: 3F00-3F1F seen at 3F20, 3F40, 3F60, ... 3FE0

				//Palette Mem Read
				if ((offset & 0x13) == 0x10) //Sprite palettes bit set, and its a colour 0 within the sprite sub-palette
				{
					// $3F10 mirrors $3F00, $3F14 mirrors $3F04, $3F18 mirrors $3F08, $3F1C mirrors $3F0C
					return state->PalMem[offset & 0x0F];
				}
				else
					return state->PalMem[offset];
			}
			else
			{
				wasPalRead = false;
				return readInternal(state, PPUAddress);
			}
		}

		//Handles a $2007 write
		void writeBy2007(PPUState *state, unsigned char value)
		{
			unsigned short int address = state->BGScrollCounters;

			unsigned short int fv = (state->PAR & 0x3);
			address |= (fv << 12);

			PPUCore::AddressSpace::write(state, address, value);
		}

		void write(PPUState *state, unsigned short int address, unsigned char value)
		{
			if (address < 0x2000)
			{
				//CHR Mem Write
				if (state->CHRMemPointer == state->InternalCHRRam)
				{
					state->InternalCHRRam[address] = value;
				}
			}
			else if (address < 0x3F00)
			{
				//NT/AT Mem Write
				unsigned short int offset = address & 0x0FFF;  //Takes care of mirroring of 2000-2FFF which is also seen at 3000-3EFF

				unsigned char *NameTableMemPointer = (unsigned char*)(state->NameTableMemPointers[offset >> 10]);

				NameTableMemPointer[offset & 0x3FF] = value;
			}
			else
			{
				//Palette Mem Write
				//   Bsspp
				unsigned short int offset = address & 0x1F;  //Takes care of mirroring of pal mem: 3F00-3F1F seen at 3F20, 3F40, 3F60, ... 3FE0

				if ((offset & 3) == 0)
				{
					// $3F10 mirrors $3F00
					// $3F14 mirrors $3F04
					// $3F18 mirrors $3F08
					// $3F1C mirrors $3F0C
					state->PalMem[offset & 0xC] = value;
					state->PalMem[(offset & 0x0C) | 0x10] = value;
				}
				else
				{
					state->PalMem[offset & 0x1F] = value;
				}
			}
		}

	}
}