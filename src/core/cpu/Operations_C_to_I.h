#ifndef CPU_OPERATIONS_C_TO_I_H
#define CPU_OPERATIONS_C_TO_I_H

#include "../globals.h"

namespace CPUCore
{
	class CPUState;

	namespace Ops
	{
		void OP_CLC(CPUState* c);
		void OP_CLD(CPUState* c);
		void OP_CLI(CPUState* c);
		void OP_CLV(CPUState* c);
		void OP_CMP(CPUState* c);
		void OP_CPX(CPUState* c);
		void OP_CPY(CPUState* c);
		void OP_DEC(CPUState* c);
		void OP_DEX(CPUState* c);
		void OP_DEY(CPUState* c);
		void OP_EOR(CPUState* c);
		void OP_INC(CPUState* c);
		void OP_INX(CPUState* c);
		void OP_INY(CPUState* c);
	}
}

#include "CPUState.h"
#include "OperationsHelpers.h"


#endif