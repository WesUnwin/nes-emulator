#include "Operations_R_to_T.h"


namespace CPUCore
{
	namespace Ops
	{

		void OP_ROL_A(CPUState* c)
		{
			_BYTE carryBefore = c->P & 0x01;
			clearNZC(c);
			if (c->A & 0x80) 
				setC(c);
			c->A = (c->A << 1);
			c->A |= carryBefore;
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c); 

			c->PC++;
		}

		void OP_ROL(CPUState* c)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			_BYTE operand = c->read(effectiveAddress);
			_BYTE carryBefore = c->P & 0x01;
			clearNZC(c);
			if (operand & 0x80) setC(c);
			operand = (operand << 1);
			operand |= carryBefore;
			if (operand == 0) setZ(c);
			if (operand & 0x80) setN(c);
			c->write(effectiveAddress, operand);
		}

		void OP_ROR_A(CPUState* c)
		{
			_BYTE carryBefore = c->P & 0x01;
			clearNZC(c);
			if (c->A & 0x01) setC(c);
			c->A = (c->A >> 1);
			if(carryBefore) c->A |= 0x80;
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);

			c->PC++;
		}

		void OP_ROR(CPUState* c)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			_BYTE operand = c->read(effectiveAddress);
			_BYTE carryBefore = c->P & 0x01;
			clearNZC(c);
			if (operand & 0x01) setC(c);
			operand = (operand >> 1);
			if(carryBefore) operand |= 0x80;
			if (operand == 0) setZ(c);
			if (operand & 0x80) setN(c);
			c->write(effectiveAddress, operand);
		}

		void OP_RTI(CPUState* c)
		{
			c->P = c->pop();
			c->P &= 0xCF;  //P:NV-BDIZC  ensure bits 4 and 5 remain clear (since there is no actual BRK flag or bit 5)

			_BYTE PCLow = c->pop();
			_BYTE PCHigh = c->pop();
			c->PC = (((_WORD)PCHigh) << 8) | ((_WORD)PCLow);
			//PC after above statement will be pointing to next opcode to execute
		}

		void OP_RTS(CPUState* c)
		{
			_BYTE PCLow = c->pop();
			_BYTE PCHigh = c->pop();
			c->PC = (((_WORD)PCHigh) << 8) | ((_WORD) PCLow);
			c->PC++;
		}

		void OP_SBC(CPUState* c)
		{
			int Carry = (c->P & 0x01);
			clearNVZC(c);
			_BYTE operand = c->getOperand();

			int result = ((int)c->A) - ((int)operand) - (Carry ^ 0x01);
			
			if (result >= 0) setC(c); //Leave C clear for borrow, setting it to 1 indicates no borrow required
			
			//Overflow Evaluation
			signed char SA = c->A;
			signed char SO = operand;

			if (((SA - SO) > 127) || ((SA - SO) < -128))
				setV(c);

			c->A = result; //Store result in A

			if(c->A == 0) setZ(c);
			if(c->A & 0x80) setN(c);			
		}

		void OP_SEC(CPUState* c)
		{
			setC(c);
			c->PC++;
		}

		void OP_SED(CPUState* c)
		{
			setD(c);
			c->PC++;
		}

		void OP_SEI(CPUState* c)
		{
			setI(c);
			c->PC++;
		}

		void OP_STA(CPUState* c)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			c->write(effectiveAddress, c->A);
		}

		void OP_STX(CPUState* c)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			c->write(effectiveAddress, c->X);
		}

		void OP_STY(CPUState* c)
		{
			_WORD effectiveAddress = c->getEffectiveAddress();
			c->write(effectiveAddress, c->Y);
		}

		void OP_TAX(CPUState* c)
		{
			clearNZ(c);
			c->X = c->A;
			if (c->X == 0) setZ(c);
			if (c->X & 0x80) setN(c);
			c->PC++;
		}

		void OP_TAY(CPUState* c)
		{
			clearNZ(c);
			c->Y = c->A;
			if (c->Y == 0) setZ(c);
			if (c->Y & 0x80) setN(c);
			c->PC++;
		}

		void OP_TSX(CPUState* c)
		{
			clearNZ(c);
			c->X = c->S;
			if (c->X == 0) setZ(c);
			if (c->X & 0x80) setN(c);
			c->PC++;
		}

		void OP_TXA(CPUState* c)
		{
			clearNZ(c);
			c->A = c->X;
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);
			c->PC++;
		}

		void OP_TXS(CPUState* c)
		{
			c->S = c->X;
			c->PC++;
		}

		void OP_TYA(CPUState* c)
		{
			clearNZ(c);
			c->A = c->Y;
			if (c->A == 0) setZ(c);
			if (c->A & 0x80) setN(c);
			c->PC++;
		}
	}
}