#include "Operations_Undoc_M_to_Z.h"


namespace CPUCore
{
	namespace Ops
	{
		void OP_NOP_AMODE(CPUState* c)   //handles DOPs and TOPs (Double and Triple byte NOPs)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();  //Advances PC based on Address mode
			//No operation besides taking into account an address mode (operand byte is not used) in advancing PC
			//Used to implement the 2-byte/3-byte illegal NOPs
		}

		void OP_RLA(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_RRA(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_SLO(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_SRE(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_SXA(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_SYA(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_XAA(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}

		void OP_XAS(CPUState* c)
		{
			throw new InvalidOpcodeException(c);   //TODO replace with actual implementation
		}


	}
}
