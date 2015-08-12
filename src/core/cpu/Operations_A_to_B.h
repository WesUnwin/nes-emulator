#ifndef CPU_OPERATIONS_A_TO_B_H
#define CPU_OPERATIONS_A_TO_B_H

#include "../globals.h"

namespace CPUCore
{
	class CPUState;

	namespace Ops
	{
		void OP_INVALID(CPUState* c);

		void OP_ADC(CPUState* c);
		void OP_AND(CPUState* c);
		void OP_ASL_A(CPUState* c);
		void OP_ASL(CPUState* c);
		void OP_BCC(CPUState* c);
		void OP_BCS(CPUState* c);
		void OP_BEQ(CPUState* c);
		void OP_BIT(CPUState* c);
		void OP_BMI(CPUState* c);
		void OP_BNE(CPUState* c);
		void OP_BPL(CPUState* c);
		void OP_BRK(CPUState* c);
		void OP_BVC(CPUState* c);
		void OP_BVS(CPUState* c);
	}
}

#include "CPUState.h"
#include "OperationsHelpers.h"
#include "InvalidOpcodeException.h"


#endif