#ifndef CPU_ADDRESSMODES_H
#define CPU_ADDRESSMODES_H

#include "../globals.h"


namespace CPUCore
{
	class CPUState;

	//The following functions return an effective address (a CPU Address) to the operand value
	namespace AddressModes
	{
		_WORD AM_IMPLIED(CPUState* c);
		_WORD AM_IMMEDIATE(CPUState* c);
		_WORD AM_ZEROPAGE(CPUState* c);
		_WORD AM_ZEROPAGE_X(CPUState* c);
		_WORD AM_ZEROPAGE_Y(CPUState* c);
		_WORD AM_ABSOLUTE(CPUState* c);
		_WORD AM_ABSOLUTE_X(CPUState* c);
		_WORD AM_ABSOLUTE_Y(CPUState* c);
		_WORD AM_INDIRECT_ABSOLUTE(CPUState* c);
		_WORD AM_INDIRECT_X(CPUState* c);
		_WORD AM_INDIRECT_Y(CPUState* c);
	}
}

#include "CPUState.h"

#endif