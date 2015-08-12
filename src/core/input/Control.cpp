#include "Control.h"



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
	return this->pressed;
}