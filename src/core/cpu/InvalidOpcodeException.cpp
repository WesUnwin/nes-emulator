#include "InvalidOpcodeException.h"

namespace CPUCore
{


	InvalidOpcodeException::InvalidOpcodeException(CPUState* state)
	{
		this->state = state;
		this->opcode = this->state->currentOpcode; 
	}


	InvalidOpcodeException::~InvalidOpcodeException()
	{
	}


	char* InvalidOpcodeException::getMessage()
	{
		//sprintf_s(this->message, "Invalid opcode: 0x%X encountered at CPU Address: 0x%X", this->opcode, this->state->PC - 1);
		return this->message;
	}


	unsigned char InvalidOpcodeException::getOpcode()
	{
		return this->opcode;
	}

}
