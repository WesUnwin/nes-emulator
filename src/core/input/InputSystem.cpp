#include "InputSystem.h"


InputSystem::InputSystem(InputCollector* inputCollector)
{
	this->inputCollector = inputCollector;
	this->player1InputDevice = 0;
	this->player2InputDevice = 0;
}


InputSystem::~InputSystem()
{
}


void InputSystem::setInputDevice(int player, NESInputDevice* inputDevice)
{
	if (player == 1)
		this->player1InputDevice = inputDevice;
	else if (player == 2)
		this->player2InputDevice = inputDevice;
}


void InputSystem::collectPlayerInput()
{

	this->inputCollector->collectPlayerInput(this->player1InputDevice, this->player2InputDevice);
}


void InputSystem::onPort4016Write(unsigned char value)
{

	//Lowest bit of write value sets Strobe (Latch) bit
	if (this->player1InputDevice != 0)
		this->player1InputDevice->setStrobe((value & 0x01) ? true : false);
	if (this->player2InputDevice != 0)
		this->player2InputDevice->setStrobe((value & 0x01) ? true : false);
}

unsigned char InputSystem::onPort4016Read()
{
	unsigned char result = 0;
	if (this->player1InputDevice != 0)
	{
		result = this->player1InputDevice->read();
		this->player1InputDevice->clock();
	}
	return result;
}

void InputSystem::onPort4017Write(unsigned char value)
{

}

unsigned char InputSystem::onPort4017Read()
{
	unsigned char result = 0;
	if (this->player2InputDevice != 0)
	{
		result = this->player2InputDevice->read();
		this->player2InputDevice->clock();
	}
	return result;
}