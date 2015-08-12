#ifndef CPU_OPERATIONS_R_TO_T_H
#define CPU_OPERATIONS_R_TO_T_H

#include "../globals.h"



namespace CPUCore
{
	class CPUState;

	namespace Ops
	{
		void OP_ROL_A(CPUState* c);
		void OP_ROL(CPUState* c);
		void OP_ROR_A(CPUState* c);
		void OP_ROR(CPUState* c);
		void OP_RTI(CPUState* c);
		void OP_RTS(CPUState* c);
		void OP_SBC(CPUState* c);
		void OP_SEC(CPUState* c);
		void OP_SED(CPUState* c);
		void OP_SEI(CPUState* c);
		void OP_STA(CPUState* c);
		void OP_STX(CPUState* c);
		void OP_STY(CPUState* c);
		void OP_TAX(CPUState* c);
		void OP_TAY(CPUState* c);
		void OP_TSX(CPUState* c);
		void OP_TXA(CPUState* c);
		void OP_TXS(CPUState* c);
		void OP_TYA(CPUState* c);
	}
}

#include "CPUState.h"
#include "OperationsHelpers.h"

#endif