#ifndef CPU_OPERATIONS_HELPERS
#define CPU_OPERATIONS_HELPERS

#include "../globals.h"
#include "CPUState.h"

namespace CPUCore
{
	namespace Ops
	{
		inline void evalN(CPUState* c, _BYTE ac) { if (ac & 0x80) c->P |= 0x80; }

		inline void evalZ(CPUState* c, _BYTE ac) { if (ac == 0) c->P |= 2; }

		inline void evalV(CPUState* c, _BYTE a, _BYTE b)
		{
			signed int ai = (signed int)a;
			signed int bi = (signed int)b;
			if (ai + bi > 127) c->P |= 64;
			if (ai + bi < -128) c->P |= 64;
		}

		inline void evalC(CPUState* c, _BYTE a, _BYTE b)
		{
			unsigned int ai = (unsigned int)a;
			unsigned int bi = (unsigned int)b;
			if (ai + bi > 255) c->P |= 1;
		}

		//P:NV-BDIZC
		inline void clearNZ(CPUState* c) { c->P &= 0x7D; }
		inline void clearNZC(CPUState* c) { c->P &= 0x7C; }
		inline void clearNVZC(CPUState* c) { c->P &= 0x3C; }
		inline void clearNVZ(CPUState* c) { c->P &= 0x3D; }

		inline void clearN(CPUState* c) { c->P &= 0x7F; }
		inline void clearV(CPUState* c) { c->P &= 0xBF; }
		inline void clearB(CPUState* c) { c->P &= 0xEF; }
		inline void clearD(CPUState* c) { c->P &= 0xF7; }
		inline void clearI(CPUState* c) { c->P &= 0xFB; }
		inline void clearZ(CPUState* c) { c->P &= 0xFD; }
		inline void clearC(CPUState* c) { c->P &= 0xFE; }

		inline void setN(CPUState* c) { c->P |= 128; }
		inline void setV(CPUState* c) { c->P |= 64; }
		inline void setB(CPUState* c) { c->P |= 16; }
		inline void setD(CPUState* c) { c->P |= 8; }
		inline void setI(CPUState* c) { c->P |= 4; }
		inline void setZ(CPUState* c) { c->P |= 2; }
		inline void setC(CPUState* c) { c->P |= 1; }

	}
}


#endif