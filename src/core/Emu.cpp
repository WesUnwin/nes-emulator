#include "Emu.h"


Emu::Emu(Renderer* renderer, SoundPlayer* soundPlayer, InputCollector* inputCollector)
{
	this->cpu = new CPUCore::CPU(this);

	this->renderer = renderer;

	this->ppu = new PPUCore::PPU(renderer);

	this->apu = new APU(soundPlayer);

	this->rom = 0;

	this->inputSystem = new InputSystem(inputCollector);
}


Emu::~Emu()
{
}


void Emu::loadROM(iNesRom* rom)
{
	stop();

	this->rom = rom;

	if (this->rom != 0)
	{
		this->rom->setupMapper(this->cpu, this->ppu);
		this->romMapper = this->rom->getMapper();
	}
}


void Emu::start()
{

}

void Emu::stop()
{

}

//Called after passing of every 60th of a second
void Emu::doFrame()
{
	if (this->rom == 0) return; //Exit if no rom loaded

	this->inputSystem->collectPlayerInput();

	cpu->newFrame();
	bool VBlankNMI = ppu->newFrame();

	if (VBlankNMI)
		cpu->raiseNMI();

	try
	{
		cpu->doRun(262 * 113);
	}
	catch (CPUCore::InvalidOpcodeException* e)
	{
		throw e;//MessageBoxA(HWND_DESKTOP, e->getMessage(), "Invalid Opcode", MB_OK);
	}

	ppu->finishFrame();	

	ppu->requestRedraw();
}


unsigned char Emu::onCPURead(unsigned short cpuAddress)
{
	if (cpuAddress >= 0x4020)
		return this->romMapper->handleCartAccess(cpuAddress, false, 0); //most high frequency
	else if (cpuAddress >= 0x4000)
		return handleCPUtoMiscPortAccess(cpuAddress, false, 0);
	else if (cpuAddress >= 0x2000)
		return handleCPUtoPPUAccess(cpuAddress & 0x7, false, 0);
	else
		return this->cpu->getState()->ram[cpuAddress & 0x07FF];
}


void Emu::onCPUWrite(unsigned short cpuAddress, unsigned char value)
{
	if (cpuAddress < 0x2000)
		this->cpu->getState()->ram[cpuAddress & 0x07FF] = value;
	else if (cpuAddress < 0x4000)
		handleCPUtoPPUAccess(cpuAddress & 0x7, true, value);
	else if (cpuAddress < 0x4020)
		handleCPUtoMiscPortAccess(cpuAddress, true, value);
	else
		this->romMapper->handleCartAccess(cpuAddress, true, value);
}


unsigned char Emu::handleCPUtoPPUAccess(int ppuPortNum, bool write, unsigned char writeValue)
{
	unsigned char result = 0;

	//Synchronize PPU to same offset into Frame as CPU, before doing the PPU Port read/write
	int ppuClocksIntoFrame = ppu->getPPUClocksIntoFrame();

	int cpuClocksIntoFrame = cpu->getCPUClocksIntoFrame();
	int cpuPPUClocksIntoFrame = cpuClocksIntoFrame * 3;

	if (ppuClocksIntoFrame >= cpuPPUClocksIntoFrame)
	{
		//Do nothing, PPU is up to (or beyond) offset into frame as CPU
	}
	else
		ppu->advanceByPPUClocks(cpuPPUClocksIntoFrame - ppuClocksIntoFrame);

	//Perform PPU Port Read/Write
	if (write == false)
		result = this->ppu->readPort(ppuPortNum);
	else
		this->ppu->writeToPort(ppuPortNum, writeValue);

	return result; //Result of read (0 for writes)
}


unsigned char Emu::handleCPUtoMiscPortAccess(unsigned short cpuAddress, bool write, unsigned char writeValue)
{
	unsigned char result = 0;

	//Read Write to APU or Misc. hardware IO ports
	if (write == false)
	{
		//Read
		if (cpuAddress == 0x4016)
			result = this->inputSystem->onPort4016Read();
		else if (cpuAddress == 0x4017)
			result = this->inputSystem->onPort4017Read();
	}
	else
	{
		//Write
		if (cpuAddress == 0x4014)
			cpu->raiseSpriteDMA(writeValue);
		else if (cpuAddress == 0x4016)
			this->inputSystem->onPort4016Write(writeValue);
		else if (cpuAddress == 0x4017)
			this->inputSystem->onPort4017Write(writeValue);
	}
	
	return result;
}





void Emu::setInputDevice(int player, InputDevice* inputDevice)
{
	if (player == 1 || player == 2)
		this->inputSystem->setInputDevice(player, inputDevice);
}