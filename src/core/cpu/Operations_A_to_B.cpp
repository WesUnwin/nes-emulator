#include "Operations_A_to_B.h"

namespace CPUCore
{
	namespace Ops
	{
		void OP_INVALID(CPUState* c)
		{
			throw new InvalidOpcodeException(c);
		}

		void OP_ADC(CPUState* c) 
		{ 
			int Carry = (c->P & 0x01);
			clearNVZC(c);
			_BYTE operand = c->getOperand();
			
			//!evaluate V and C before modifying A
			signed char SA = c->A;
			signed char SO = operand;
			signed char SC = Carry;

			if (((SA + SO + SC) > 127) || ((SA + SO + SC) < -128)) setV(c);

			if (c->A + operand + Carry > 255) setC(c);

			c->A += operand + Carry;

			evalN(c, c->A);
			evalZ(c, c->A);
		}

		void OP_AND(CPUState* c) 
		{ 
			clearNZ(c);
			_BYTE operand = c->getOperand();
			c->A &= operand;
			evalN(c, c->A);
			evalZ(c, c->A);
		}

		void OP_ASL_A(CPUState* c) 
		{ 
			clearNZC(c);
			_WORD aw = (_WORD)c->A;
			aw = (aw << 1);

			if(aw & 0x0100) setC(c); //Carry
			c->A = (_BYTE)(aw & 0x00FF);

			evalN(c, c->A); //check for N and Z after updating A to shift
			evalZ(c, c->A);		

			c->PC++;
		}

		void OP_ASL(CPUState* c) 
		{ 
			clearNZC(c);
			_WORD effectiveAddress = c->getEffectiveAddress();

			_WORD operand = (_WORD)c->read(effectiveAddress);  //Read
			operand = (operand << 1);  //Modify
			if (operand & 0x0100) setC(c);
			_BYTE result = (_BYTE)(operand & 0x00FF);
			evalN(c, result);
			evalZ(c, result);
			c->write(effectiveAddress, result);  //Write back
		}

		void OP_BCC(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 0x01)  //P:NV-BDIZC
				c->PC += 1;  //Don't branch, on to next instruction
			else
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}				
		}

		void OP_BCS(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 0x01)  //Brach on C set
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}
			else
				c->PC += 1;  //Don't branch, on to next instruction		
		}

		void OP_BEQ(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 0x02)  //Brach on Z set
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}
			else
				c->PC += 1;  //Don't branch, on to next instruction			
		}

		void OP_BIT(CPUState* c)
		{
			clearNVZ(c);
			_WORD operand = (_WORD)c->getOperand();  //Read

			if(operand & 128) c->P |= 0x80;
			if(operand & 64) c->P |= 64;
			if(!(c->A & operand)) c->P |= 2;
		}

		void OP_BMI(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 128)  //Brach on N set
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}
			else
				c->PC += 1;  //Don't branch, on to next instruction	
		}

		void OP_BNE(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 0x02)   //P:NV-BDIZC
				c->PC += 1;  //Don't branch, on to next instruction	
			else
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}				
		}

		void OP_BPL(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 128)
				c->PC += 1;  //Don't branch, on to next instruction	
			else
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;  //PC now points to opcode after branch instruction 

				c->PC += delta;
			}
		}

		void OP_BRK(CPUState* c) 
		{ 
			c->PC += 2; //Return address is always 2 bytes after the BRK opcode 
			//(pretend an extra padding byte exists after each BRK opcode)

			c->push((c->PC >> 8) & 0x00FF); //Push PC High
			c->push(c->PC & 0x00FF); //Push PC Low
			c->push(c->P | 0x30); //P is pushed with bits 5 and 4 set  P:NV-BDIZC    (distinguishes BRK from IRQ Interrupts)

			unsigned char pcLowByte = c->read(0xFFFE);  //0xFFFE is the address of the low byte of the IRQ vector
			unsigned char pcHighByte = c->read(0xFFFF); //0xFFFF is the address of the high byte of the IRQ vector
			c->PC = ((((unsigned short)pcHighByte) << 8) | ((unsigned short)pcLowByte));
		}

		void OP_BVC(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 64)  //P:NV-BDIZC
				c->PC += 1;  //Don't branch, on to next instruction
			else
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}
		}

		void OP_BVS(CPUState* c) 
		{ 
			c->PC++;  //after this points to the operand byte (a PC relative value)
			if (c->P & 64)  //Brach on V set
			{
				signed short delta = (signed char)c->read(c->PC);
				c->PC += 1;

				c->PC += delta;
			}
			else
				c->PC += 1;  //Don't branch, on to next instruction	
		}

	}
}