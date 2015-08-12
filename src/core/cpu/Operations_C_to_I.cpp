#include "Operations_C_to_I.h"


namespace CPUCore
{
	namespace Ops
	{

		void OP_CLC(CPUState* c)
		{
			clearC(c);
			c->PC++;
		}

		void OP_CLD(CPUState* c)
		{
			clearD(c);
			c->PC++;
		}

		void OP_CLI(CPUState* c)
		{
			clearI(c);
			c->PC++;
		}

		void OP_CLV(CPUState* c)
		{
			clearV(c);
			c->PC++;
		}

		void OP_CMP(CPUState* c)
		{
			clearNZC(c);
			_BYTE operand = c->getOperand();

			_BYTE result = c->A- operand;

			if (c->A >= operand) setC(c);  //No Borrow / Carry = 1, elsewise Carry remains 0 (for Borrow)
			if (result == 0) setZ(c);
			if (result & 0x80) setN(c);
		}

		void OP_CPX(CPUState* c)
		{
			clearNZC(c);
			_BYTE operand = c->getOperand();

			_BYTE result = c->X - operand;
			
			if (c->X >= operand) setC(c);  //No Borrow / Carry = 1, elsewise Carry remains 0 (for Borrow)
			if (result == 0) setZ(c);
			if (result & 0x80) setN(c);
		}

		void OP_CPY(CPUState* c)
		{
			clearNZC(c);
			_BYTE operand = c->getOperand();

			_BYTE result = c->Y - operand;

			if (c->Y >= operand) setC(c);  //No Borrow / Carry = 1, elsewise Carry remains 0 (for Borrow)
			if (result == 0) setZ(c);
			if (result & 0x80) setN(c);
		}

		void OP_DEC(CPUState* c)
		{
			clearNZ(c);
			_WORD effectiveAddress = c->getEffectiveAddress();
			_BYTE operand = c->read(effectiveAddress);
			operand--;
			if (operand == 0) setZ(c);
			if (operand & 0x80) setN(c);
			c->write(effectiveAddress, operand);
		}

		void OP_DEX(CPUState* c)
		{
			clearNZ(c);
			c->X -= 1;
			if (c->X == 0) setZ(c);
			if (c->X & 0x80) setN(c);
			c->PC++;
		}

		void OP_DEY(CPUState* c)
		{
			clearNZ(c);
			c->Y -= 1;
			if (c->Y == 0) setZ(c);
			if (c->Y & 0x80) setN(c);
			c->PC++;
		}

		void OP_EOR(CPUState* c)
		{
			clearNZ(c);
			_BYTE operand = c->getOperand();
			c->A ^= operand;
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);
		}

		void OP_INC(CPUState* c)
		{
			clearNZ(c);
			_WORD effectiveAddress = c->getEffectiveAddress();
			_BYTE operand = c->read(effectiveAddress);
			operand++;
			if(operand == 0) setZ(c);
			if(operand & 0x80) setN(c);
			c->write(effectiveAddress, operand);
		}

		void OP_INX(CPUState* c)
		{
			clearNZ(c);
			c->X++;
			if (c->X == 0) setZ(c);
			if (c->X & 0x80) setN(c);
			c->PC++;
		}

		void OP_INY(CPUState* c)
		{
			clearNZ(c);
			c->Y++;
			if (c->Y == 0) setZ(c);
			if (c->Y & 0x80) setN(c);
			c->PC++;
		}


	}
}