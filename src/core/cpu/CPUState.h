#ifndef CPU_STATE_H
#define CPU_STATE_H

#include "../globals.h"
#include "CPUReadWriteListener.h"


//#include "..\NES\NES\Emu.h"

namespace CPUCore
{
	class CPUAddressSpace;

	class CPUState
	{
	public:
		CPUState();
		~CPUState();

		unsigned int InterruptFlags;  //If zero no interrupts are being raised

		bool checkForRESET();
		void raiseRESET();
		void clearRESET();

		bool checkForNMI();
		void raiseNMI();
		void clearNMI();

		bool checkForIRQ();
		void raiseIRQ();
		void clearIRQ();

		bool checkForSpriteDMA();
		void raiseSpriteDMA(_BYTE pageNumber);
		void clearSpriteDMA();

		int SpriteDMACyclesLeft;
		_BYTE SpriteDMAByte;
		_WORD SpriteDMACPUAddress; //Current CPU Address of next byte to read from

		CPUReadWriteListener* readWriteListener;

		_BYTE A;
		_BYTE X;
		_BYTE Y;
		_BYTE P;  //P:NV-BDIZC
		_BYTE S;
		_WORD PC;

		_BYTE currentOpcode;
		_BYTE previousOpcode;
		
		_BYTE ram[2048];

		int readOpcode(); //Sets currentOpcode after reading byte at PC, then returns # of clocks required to execute the instruction (based on opcode value)

		_WORD getEffectiveAddress();
		_BYTE getOperand();

		void executeCurrentInstruction();

		_BYTE read(_WORD cpuAddress);
		void write(_WORD cpuAddress, _BYTE value);
		void push(_BYTE value);
		_BYTE pop();

//		Emu* emu;
	private:

	};

}

#include "Opcodes.h"

#endif