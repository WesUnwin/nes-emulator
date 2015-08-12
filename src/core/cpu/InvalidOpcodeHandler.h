#ifndef CPU_INVALIDOPCODEHANDLER_H
#define CPU_INVALIDOPCODEHANDLER_H

#include "InvalidOpcodeException.h"


namespace CPUCore
{
	class InvalidOpcodeHandler
	{
	public:
		InvalidOpcodeHandler();
		~InvalidOpcodeHandler();

		virtual void handleInvalidOpcodeException(InvalidOpcodeException* e) = 0;
	};


	class DefaultInvalidOpcodeHandler : public InvalidOpcodeHandler
	{
	public:
		DefaultInvalidOpcodeHandler();
		~DefaultInvalidOpcodeHandler();

		virtual void handleInvalidOpcodeException(InvalidOpcodeException* e)
		{
			throw e;
		}
	};



}


#endif