#include "CPU.h"

namespace CPUCore
{
	CPU::CPU(CPUReadWriteListener* listener)
	{
		this->state = new CPUState();
		this->state->readWriteListener = listener;
		this->cpuClocksIntoFrame = 0;

		this->invalidOpcodeHandler = new DefaultInvalidOpcodeHandler();
	}


	CPU::~CPU()
	{
	}


	void CPU::newFrame()
	{
		this->cpuClocksIntoFrame = 0;
	}


	void CPU::raiseRESET()
	{
		this->state->raiseRESET();
	}


	void CPU::raiseNMI()
	{
		this->state->raiseNMI();
	}


	void CPU::raiseIRQ()
	{
		this->state->raiseIRQ();
	}


	void CPU::raiseSpriteDMA(int pageNumber)
	{
		this->state->raiseSpriteDMA( pageNumber & 0xFF); //pageNumber passed in as int so caller does not need to declare _BYTE
	}


	void CPU::doRun(int cpuClocks)
	{
		if (cpuClocks <= 0)
			return;

		int clocksLeft = cpuClocks;

		bool runComplete = false;
		do
		{
			//Process the highest priority interrupt (if any) first
			if (state->InterruptFlags != 0)  //Skips calling processInterrupts if no interrupts at all (for performance) (99% of the time there are no interrupts)
				processInterrupts(clocksLeft);

			state->previousOpcode = state->currentOpcode;

			int clocksForInstruction = state->readOpcode();

			if (clocksLeft >= clocksForInstruction)
			{
				//Do Instruction

				//Execute Instruction's operation
				try
				{
					this->state->executeCurrentInstruction();
				}
				catch (InvalidOpcodeException* e)
				{
					_BYTE prop = this->state->previousOpcode;
					_BYTE op = this->state->currentOpcode;
					this->invalidOpcodeHandler->handleInvalidOpcodeException(e);
				}
				
				//Update CPU Clock counter
				this->cpuClocksIntoFrame += clocksForInstruction;
				clocksLeft -= clocksForInstruction;
			}
			else
				runComplete = true;

		} while (runComplete == false);

		this->cpuClocksIntoFrame += cpuClocks;
	}


	void CPU::processInterrupts(int clocksLeft)
	{
		//Process High Priority Raised Interrupt (If any) before attempting next instruction
		//This does not execute the handler, just sets up the PC to cause the handler to get executed next
		if (state->checkForRESET())  //RESET  - highest priority interrupt
		{
			unsigned char pcLowByte = state->read(0xFFFC);  //0xFFFC is the address of the low byte of the RESET vector
			unsigned char pcHighByte = state->read(0xFFFD); //0xFFFD is the address of the high byte of the RESET vector
			state->PC = ((((unsigned short)pcHighByte) << 8) | ((unsigned short)pcLowByte));

			state->clearRESET();
		}
		else if (state->checkForNMI())  //NMI
		{
			state->push((state->PC >> 8) & 0x00FF); //Push PC High
			state->push(state->PC & 0x00FF); //Push PC Low
			state->push(state->P | 0x20);  //Push Processor Flags (pushed value has bit 5 set)

			unsigned char pcLowByte = state->read(0xFFFA);  //0xFFFA is the address of the low byte of the NMI vector
			unsigned char pcHighByte = state->read(0xFFFB); //0xFFFB is the address of the high byte of the NMI vector
			state->PC = ((((unsigned short)pcHighByte) << 8) | ((unsigned short)pcLowByte));

			state->clearNMI();
		}
		else if (state->checkForIRQ())  //IRQ
		{
			state->push((state->PC >> 8) & 0x00FF); //Push PC High
			state->push(state->PC & 0x00FF); //Push PC Low
			state->push(state->P | 0x20);  //Push Processor Flags (pushed value has bit 5 set)

			unsigned char pcLowByte = state->read(0xFFFE);  //0xFFFE is the address of the low byte of the IRQ vector
			unsigned char pcHighByte = state->read(0xFFFF); //0xFFFF is the address of the high byte of the IRQ vector
			state->PC = ((((unsigned short)pcHighByte) << 8) | ((unsigned short)pcLowByte));

			state->clearIRQ();
		}	
		else if (state->checkForSpriteDMA())
		{
			while (state->SpriteDMACyclesLeft > 0 && clocksLeft >= 1)
			{
				if (state->SpriteDMACyclesLeft >= 513)
				{
					//Wastes a cycle (cycle used to Halt CPU for duration of DMA transfer)
				}
				else if ((state->SpriteDMACyclesLeft & 0x01) == 0) //An even clock (between 2-512)
				{
					//Read a Byte from CPU Ram
					state->SpriteDMAByte = state->read(state->SpriteDMACPUAddress);
					state->SpriteDMACPUAddress++;
				}
				else //An odd clock (1-511)
				{
					//Write the byte to PPU register $2004
					state->write(0x2004, state->SpriteDMAByte);
					if (state->SpriteDMACyclesLeft == 1)
					{
						state->clearSpriteDMA(); //Sprite DMA Transfer will be finished after this
					}
				}

				state->SpriteDMACyclesLeft--;

				clocksLeft--;
				this->cpuClocksIntoFrame++;
			}		
		}
	}


	int CPU::getCPUClocksIntoFrame()
	{
		return this->cpuClocksIntoFrame;
	}


	CPUState* CPU::getState()
	{
		return this->state;
	}


	void CPU::setState(CPUState* newstate)
	{
		this->state = newstate;
	}

	void CPU::setInvalidOpcodeHandler(InvalidOpcodeHandler* handler)
	{
		this->invalidOpcodeHandler = handler;
	}

}