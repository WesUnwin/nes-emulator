#ifndef PPU_ADDRESSSPACE_H
#define PPU_ADDRESSSPACE_H

#include "PPUState.h"


namespace PPUCore
{
	namespace AddressSpace
	{
		//Done by PPU during rendering, reads Pattern Tables and Name tables (does not read pallette memory)
		unsigned char readInternal(PPUState *state, unsigned short int PPUAddress);

		//Used for reading the (unbuffered) value for Port $2007 reads (will read palette memory where applicable)
		unsigned char readBy2007(PPUState *state, bool& wasPalRead, unsigned char& underlyingNTByte);

		//Handles a port $2007 write, the address to write to is based on the current state of the PPU Scroll Counters (or is it latches?)s
		void writeBy2007(PPUState *state, unsigned char value);

		void write(PPUState *state, unsigned short int address, unsigned char value);

	}
}

#endif
