#include "Control.h"
#include <iostream>

using namespace std;

void InputControl::setHostKeyBinding(HostInputDevice* hostInputDevice, int hostKey)
{
	this->hostInputDevice = hostInputDevice;
	this->hostKey = hostKey;
}

ButtonControl::~ButtonControl()
{
	this->pressed = false;
}

void ButtonControl::setState(bool isPressed)
{
	this->pressed = isPressed;
}

bool ButtonControl::isPressed()
{
	cout << "checking if button is pressed " << this->getName() << std::endl;

	if(this->hostInputDevice == 0)
		cout << "NO HOST KEY BINDING FOR THIS CONTROL: " << this->getName() << std::endl;
	if(this->hostInputDevice == 0)
		return false;
	else
		return this->hostInputDevice->isKeyDown(this->hostKey);
}