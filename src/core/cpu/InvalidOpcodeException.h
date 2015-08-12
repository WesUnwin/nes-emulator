#ifndef CPU_INVALIDOPCODEEXCEPTION_H
#define CPU_INVALIDOPCODEEXCEPTION_H

#include "CPUState.h"

#include <stdio.h>




namespace CPUCore
{

	class InvalidOpcodeException
	{
	public:
		InvalidOpcodeException(CPUState* state);
		~InvalidOpcodeException();

		char* getMessage();

		unsigned char getOpcode();

	private:
		char message[256];
		CPUState* state;

		unsigned char opcode;
	};

}




#endif

