#ifndef PPU_H
#define PPU_H

#include "PPUState.h"
#include "Renderer.h"
#include "AddressSpace.h"
#include "ScrollCounterOps.h"
#include "FrameSequence.h"
#include "ScanlineRoutines.h"

namespace PPUCore
{
	class PPU
	{
	public:
		PPU(Renderer* renderer);
		~PPU();

		bool newFrame(); //Returns true if a NMI should be raised

		void advanceByPPUClocks(int ppuClocks);
		void finishFrame();
		void requestRedraw();

		int getPPUClocksIntoFrame();

		unsigned char readPort(int PortNum);
		void writeToPort(int PortNum, unsigned char value);

		void incrementVRAMAddress(PPUState* state);

		void setCHRMemPointer(unsigned char* chrMem);

		void setNTMirroring(PPUState::NTMIRRORING mirroringType);

		PPUState* getState();
	private:
		PPUState *state;
	};
}
#endif