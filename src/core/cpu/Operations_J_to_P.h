#ifndef CPU_OPERATIONS_J_TO_P_H
#define CPU_OPERATIONS_J_TO_P_H

#include "../globals.h"



namespace CPUCore
{
	class CPUState;

	namespace Ops
	{
		void OP_JMP(CPUState* c);
		void OP_JSR(CPUState* c);
		void OP_LDA(CPUState* c);
		void OP_LDX(CPUState* c);
		void OP_LDY(CPUState* c);
		void OP_LSR_A(CPUState* c);
		void OP_LSR(CPUState* c);
		void OP_NOP(CPUState* c);
		void OP_ORA(CPUState* c);
		void OP_PHA(CPUState* c);
		void OP_PHP(CPUState* c);
		void OP_PLA(CPUState* c);
		void OP_PLP(CPUState* c);
	}
}

#include "CPUState.h"
#include "OperationsHelpers.h"


#endif

