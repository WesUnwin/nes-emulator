#ifndef CPU_OPERATIONS_UNDOC_M_TO_Z_H
#define CPU_OPERATIONS_UNDOC_M_TO_Z_H


namespace CPUCore
{
	class CPUState;

	namespace Ops
	{

		void OP_NOP_AMODE(CPUState* c); //handles DOPs and TOPs (Double and Triple byte NOPs)

		void OP_RLA(CPUState* c);

		void OP_RRA(CPUState* c);

		void OP_SLO(CPUState* c);

		void OP_SRE(CPUState* c);

		void OP_SXA(CPUState* c);

		void OP_SYA(CPUState* c);

		void OP_XAA(CPUState* c);

		void OP_XAS(CPUState* c);

	}
}

#include "CPUState.h"
#include "InvalidOpcodeException.h"

#endif