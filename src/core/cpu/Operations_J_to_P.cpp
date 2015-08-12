#include "Operations_J_to_P.h"


namespace CPUCore
{
	namespace Ops
	{
		void OP_JMP(CPUState* c) //Used with absolute or indirect address modes
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			c->PC = effectiveAddress;  //here the effective address (the 2 operand bytes) are the address to jump to
		}

		void OP_JSR(CPUState* c)  //Only used with absolute addressing
		{
			_WORD effectiveAddress = c->getEffectiveAddress(); 	//after, PC points to opcode of next instruction
			c->PC--;
			c->push((_BYTE)(c->PC >> 8));   //push high byte of PC first
			c->push((_BYTE)(c->PC & 0x00FF));
			c->PC = effectiveAddress;    //effective address is CPU Address of the sub-routine being called
		}

		void OP_LDA(CPUState* c)
		{
			clearNZ(c);
			c->A = c->getOperand();
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);
		}

		void OP_LDX(CPUState* c)
		{
			clearNZ(c);
			c->X = c->getOperand();
			if (c->X == 0) setZ(c);
			if (c->X & 0x80) setN(c);
		}

		void OP_LDY(CPUState* c)
		{
			clearNZ(c);
			c->Y = c->getOperand();
			if (c->Y == 0) setZ(c);
			if (c->Y & 0x80) setN(c);
		}

		void OP_LSR_A(CPUState* c)
		{
			clearNZC(c);
			if (c->A & 0x01) setC(c); //Lowest bit would shift into C
			c->A = (c->A >> 1);
			if (c->A == 0) setZ(c);
			clearN(c);  //N will always be zero due to shift
			c->PC++;
		}

		void OP_LSR(CPUState* c)
		{
			clearNZC(c);
			_WORD effectiveAddress = c->getEffectiveAddress();
			_BYTE operand = c->read(effectiveAddress);
			if (operand & 0x01) setC(c); //Lowest bit would shift into C
			operand = (operand >> 1);
			if (operand == 0) setZ(c);
			clearN(c);  //N will always be zero due to shift
			c->write(effectiveAddress, operand);
		}

		void OP_NOP(CPUState* c)
		{
			//No-Operation
			c->PC++;
		}

		void OP_ORA(CPUState* c)
		{
			clearNZ(c);
			c->A |= c->getOperand();
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);
		}

		void OP_PHA(CPUState* c)
		{
			c->push(c->A);
			c->PC++;
		}

		void OP_PHP(CPUState* c)
		{
			c->push(c->P | 0x30);  //The pushed value of P will have bits 5 and 4 set
			c->PC++;
		}

		void OP_PLA(CPUState* c)
		{
			clearNZ(c);

			c->A = c->pop();

			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);

			c->PC++;
		}

		void OP_PLP(CPUState* c)
		{
			c->P = c->pop();
			c->P &= 0xCF;  //P:NV-BDIZC  ensure bits 4 and 5 remain clear (since there is no actaul BRK flag or bit 5)
			
			c->PC++;
		}
		
	}
}