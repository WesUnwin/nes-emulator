#include "AddressModes.h"


namespace CPUCore
{
	namespace AddressModes
	{
		_WORD AM_IMPLIED(CPUState* c)
		{
			c->PC += 1; //Next opcode
			return 0;
		}

		_WORD AM_IMMEDIATE(CPUState* c)
		{
			_WORD effectiveAddress = c->PC + 1;
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ZEROPAGE(CPUState* c)
		{
			_WORD effectiveAddress = c->read(c->PC + 1); //upper byte will be zero
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ZEROPAGE_X(CPUState* c)  //<$AA, X
		{
			_BYTE AA = c->read(c->PC + 1);
			AA += c->X;   //WRAP AROUND ADDITION OCCURS HERE
			_WORD effectiveAddress = (_WORD)AA;
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ZEROPAGE_Y(CPUState* c) //<$AA, Y    //TODO Not sure if this exists?
		{
			_BYTE AA = c->read(c->PC + 1);
			AA += c->Y;   //WRAP AROUND ADDITION OCCURS HERE
			_WORD effectiveAddress = (_WORD)AA;
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ABSOLUTE(CPUState* c)  //$BBAA
		{
			_BYTE lowerAddressByte = c->read(c->PC + 1);
			_BYTE upperAddressByte = c->read(c->PC + 2);
			_WORD effectiveAddress = (((_WORD)upperAddressByte) << 8) | ((_WORD)lowerAddressByte);
			c->PC += 3; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ABSOLUTE_X(CPUState* c)  //$BBAA, X
		{
			_BYTE lowerAddressByte = c->read(c->PC + 1);
			_BYTE upperAddressByte = c->read(c->PC + 2);
			_WORD effectiveAddress = ((_WORD)upperAddressByte << 8) | ((_WORD)lowerAddressByte);
			effectiveAddress += c->X;   //Confirmed: upper byte is allowed to increment on lower byte + X overflowing
			c->PC += 3; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_ABSOLUTE_Y(CPUState* c)  //$BBAA, Y
		{
			_BYTE lowerAddressByte = c->read(c->PC + 1);
			_BYTE upperAddressByte = c->read(c->PC + 2);
			_WORD effectiveAddress = (((_WORD)upperAddressByte) << 8) | ((_WORD)lowerAddressByte);
			effectiveAddress += c->Y;
			c->PC += 3; //on to next opcode
			return effectiveAddress;
		}

		_WORD AM_INDIRECT_ABSOLUTE(CPUState* c) //Used by JMP ($BBAA)
		{
			_BYTE lowerAddressByte = c->read(c->PC + 1);
			_BYTE upperAddressByte = c->read(c->PC + 2);
			_WORD vectorAddress_ForLower = (((_WORD)upperAddressByte) << 8) | ((_WORD)lowerAddressByte);

			lowerAddressByte++; //VERY IMPORTANT on a 6502, when reading upper byte (at PC + 1) the +1 is added to lower byte with wrap around addition
			_WORD vectorAddress_ForUpper = (((_WORD)upperAddressByte) << 8) | ((_WORD)lowerAddressByte);

			_BYTE effectiveAddressLower = c->read(vectorAddress_ForLower);
			_BYTE effectiveAddressUpper = c->read(vectorAddress_ForUpper); 
			_WORD effectiveAddress = (((_WORD)effectiveAddressUpper) << 8) | ((_WORD)effectiveAddressLower);

			c->PC += 3; //Won't actually matter since PC is later set to effective address
			return effectiveAddress;
		}

		_WORD AM_INDIRECT_X(CPUState* c)  //($AA,X)
		{
			_BYTE AA = c->read(c->PC + 1); 
			AA += c->X;
			_BYTE effectiveAddress_Low = c->read((_WORD)AA);
			_BYTE effectiveAddress_High = c->read(((_WORD)((AA + 1) & 0x00FF))); //CONFIRMED WRAP AROUND ADDITION USED HERE
			_WORD effectiveAddress = (((_WORD)effectiveAddress_High) << 8) | ((_WORD)effectiveAddress_Low);
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}
		
		_WORD AM_INDIRECT_Y(CPUState* c)  //($AA),Y
		{
			_BYTE AA = c->read(c->PC + 1); 
			_BYTE effectiveAddress_Low = c->read((_WORD)AA);

			AA++;  //wrap around addition is used here 
			_BYTE effectiveAddress_High = c->read((_WORD)AA); //AA now points to upper byte of 16-bit address to be added with Y

			_WORD effectiveAddress = ((_WORD)effectiveAddress_High << 8) | ((_WORD)effectiveAddress_Low);

			effectiveAddress += c->Y;  //Confirmed: no-wrap arround addition here, allow upper byte to increment when adding Y to lower byte
			
			c->PC += 2; //on to next opcode
			return effectiveAddress;
		}
	}
}

