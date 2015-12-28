#ifndef EMU_H
#define EMU_H

#include "cpu/CPU.h"
#include "cpu/CPUState.h"
#include "cpu/CPUReadWriteListener.h"
#include "cpu/InvalidOpcodeException.h"

#include "ppu/PPU.h"

#include "apu/APU.h"
#include "apu/SoundPlayer.h"

#include "cart/iNesRom.h"

#include "input/InputSystem.h"
#include "input/InputDevice.h"
#include "input/InputCollector.h"


class Emu : public CPUReadWriteListener
{
public:
	Emu(Renderer* renderer, SoundPlayer* soundPlayer, InputCollector* inputCollector);
	~Emu();


	void loadROM(iNesRom* rom);

	void start();
	void stop();

	void doFrame(); // Called every frame (60th of a sec) no matter what

	virtual unsigned char onCPURead(unsigned short cpuAddress);
	virtual void onCPUWrite(unsigned short cpuAddress, unsigned char value);


	unsigned char handleCPUtoPPUAccess(int ppuPortNum, bool write, unsigned char writeValue);
	unsigned char handleCPUtoMiscPortAccess(unsigned short cpuAddress, bool write, unsigned char writeValue);

	void setInputDevice(int player, InputDevice* inputDevice);

private:
	CPUCore::CPU* cpu;
	PPUCore::PPU* ppu;
	APU* apu;


	Renderer* renderer;
	iNesRom* rom;
	CartMapper* romMapper;
	InputSystem* inputSystem;

};

#endif
