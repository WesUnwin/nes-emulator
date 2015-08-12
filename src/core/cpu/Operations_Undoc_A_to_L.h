#ifndef CPU_OPERATIONS_UNDOC_A_TO_L_H
#define CPU_OPERATIONS_UNDOC_A_TO_L_H


namespace CPUCore
{
	class CPUState;

	namespace Ops
	{
		void OP_AAC(CPUState* c);  //a.k.a. ANC

		void OP_AAX(CPUState* c);  //a.k.a. SAX

		void OP_ARR(CPUState* c);

		void OP_ASR(CPUState* c);

		void OP_ATX(CPUState* c);

		void OP_AXA(CPUState* c);

		void OP_AXS(CPUState* c);

		void OP_DCP(CPUState* c);

		void OP_ISC(CPUState* c);  //a.k.a. ISB, INS

		void OP_KIL(CPUState* c);

		void OP_LAR(CPUState* c);  //a.k.a. LAE

		void OP_LAX(CPUState* c);  

	}
}

#include "CPUState.h"
#include "InvalidOpcodeException.h"

#endif