#ifndef CPU_H
#define CPU_H


#include "../globals.h"
#include "CPUState.h"
#include "CPUReadWriteListener.h"
#include "Opcodes.h"
#include "InvalidOpcodeException.h"
#include "InvalidOpcodeHandler.h"


namespace CPUCore
{
	
	class CPU
	{
	public:
 		CPU(CPUReadWriteListener* listener);
		~CPU();

		CPUState* getState();
		void setState(CPUState* newstate);

		void newFrame();

		void raiseRESET();
		void raiseNMI();
		void raiseIRQ();

		void raiseSpriteDMA(int pageNumber);

		void doRun(int cpuClocks);

		int getCPUClocksIntoFrame();

		void setInvalidOpcodeHandler(InvalidOpcodeHandler* handler);


	private:
		CPUState* state;
		int cpuClocksIntoFrame;

		void processInterrupts(int clocksLeft);
		
		InvalidOpcodeHandler* invalidOpcodeHandler;
	};



}


#endif