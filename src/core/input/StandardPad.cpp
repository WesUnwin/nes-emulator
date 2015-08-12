#include "StandardPad.h"



StandardPad::StandardPad() : description(std::string("Standard Pad"))
{
	buttons[0] = new ButtonControl("A");
	buttons[1] = new ButtonControl("B");
	buttons[2] = new ButtonControl("Select");
	buttons[3] = new ButtonControl("Start");
	buttons[4] = new ButtonControl("Up");
	buttons[5] = new ButtonControl("Down");
	buttons[6] = new ButtonControl("Left");
	buttons[7] = new ButtonControl("Right");

	latch = false;
}


StandardPad::~StandardPad()
{
}


const std::string& StandardPad::getDeviceDescription()
{
	return description;
}

int StandardPad::getNumberOfControls()
{
	return 8;
}

InputControl* StandardPad::getControl(int index)
{
	return buttons[index];
}

void StandardPad::setStrobe(bool s)
{
	if (this->latch == false && s == false) //Latch being set
	{
		//Set Shift Register to current snapshot of the 8 buttons
		this->shiftRegister = 0;
		for (int i = 0; i < 8; i++)
		{
			bool pressed = this->buttons[i]->isPressed();
			this->shiftRegister |= ((pressed ? 0 : 1) << i);
		}
	}
	
	this->latch = !s;
}

void StandardPad::clock()
{
	if (this->latch) this->shiftRegister = (this->shiftRegister >> 1);
}

unsigned char StandardPad::read()
{
	unsigned char result = 0x40;
	if (latch)
	{
		result |= (this->shiftRegister & 0x01);
	}
	else
	{
		bool APressed = buttons[0]->isPressed();
		result |= (APressed ? 0x01 : 0);
	}

	result ^= 0x01;  //Invert lowest bit

	return result;
}