#ifndef PPU_SCROLLCOUNTEROPS_H
#define PPU_SCROLLCOUNTEROPS_H

#include "PPUState.h"

namespace PPUCore
{
	namespace ScrollCounterOps
	{
		void port0Write(PPUState *state, unsigned char value);
		void port5Write_0(PPUState *state, unsigned char value);
		void port5Write_1(PPUState *state, unsigned char value);
		void port6Write_0(PPUState *state, unsigned char value);
		void port6Write_1(PPUState *state, unsigned char value);

		void loadLatchedHorizontalCounters(PPUState* state);
		void loadLatchedVerticalCounters(PPUState* state);

		//Called when ready to move onto rendering next scanline
		void incrementVerticalCounters(PPUState* state);

		void incrementHorizontalScrollCounters(PPUState* state);

		//Called by both 2007 reads/writes (with increment = 1) 
		void incrementScrollCountersBy1(PPUState *state);

		//Called by both 2007 reads/writes (with increment = 32)
		void incrementScrollCountersBy32(PPUState *state);
	}
}

#endif
