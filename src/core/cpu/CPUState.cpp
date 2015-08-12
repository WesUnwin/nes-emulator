#include "CPUState.h"


namespace CPUCore
{

	CPUState::CPUState()
	{
		this->InterruptFlags = 0;
		this->raiseRESET();
	}


	CPUState::~CPUState()
	{
	}


	int CPUState::readOpcode()
	{
		this->currentOpcode = read(PC);
		return opcodes[this->currentOpcode].CpuClocks;  //Return Clocks Required for this instruction
	}

	
	_WORD CPUState::getEffectiveAddress()
	{
		return opcodes[this->currentOpcode].addressModeRoutine(this);
	}


	_BYTE CPUState::getOperand()
	{
		return this->read(this->getEffectiveAddress());
	}


	void CPUState::executeCurrentInstruction()
	{
		opcodes[this->currentOpcode].operationRoutine(this);
	}


	_BYTE CPUState::read(_WORD cpuAddress)
	{
		return this->readWriteListener->onCPURead(cpuAddress);
	}


	void CPUState::write(_WORD cpuAddress, _BYTE value)
	{
		this->readWriteListener->onCPUWrite(cpuAddress, value);
	}


	void CPUState::push(_BYTE value)
	{
		write(0x0100 + ((_WORD)S), value);
		S--;
	}

	_BYTE CPUState::pop()
	{
		S++;
		return read( 0x0100 + ((_WORD) S) );
	}


	bool CPUState::checkForRESET()
	{
		if (this->InterruptFlags & 1)
			return true;
		else
			return false;
	}

	void CPUState::raiseRESET()
	{
		this->InterruptFlags |= 1;
	}

	void CPUState::clearRESET()
	{
		this->InterruptFlags &= 0xFFFFFFFE;
	}

	bool CPUState::checkForNMI()
	{
		if (this->InterruptFlags & 2)
			return true;
		else
			return false;
	}

	void CPUState::raiseNMI()
	{
		this->InterruptFlags |= 2;
	}

	void CPUState::clearNMI()
	{
		this->InterruptFlags &= 0xFFFFFFFD;
	}

	bool CPUState::checkForIRQ()
	{
		if (this->InterruptFlags & 4)
			return true;
		else
			return false;
	}

	void CPUState::raiseIRQ()
	{
		this->InterruptFlags |= 4;
	}

	void CPUState::clearIRQ()
	{
		this->InterruptFlags &= 0xFFFFFFFB;
	}


	bool CPUState::checkForSpriteDMA()
	{
		if (this->InterruptFlags & 8)
			return true;
		else
			return false;
	}

	void CPUState::raiseSpriteDMA(_BYTE pageNumber)
	{
		this->InterruptFlags |= 8;
		this->SpriteDMACyclesLeft = 513;
		this->SpriteDMACPUAddress = (((_WORD)pageNumber) << 8);
	}

	void CPUState::clearSpriteDMA()
	{
		this->InterruptFlags &= 0xFFFFFFF7;
		this->SpriteDMACyclesLeft = 0;
	}

}

