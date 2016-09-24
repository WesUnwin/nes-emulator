#include "LinuxKeyboardInputDevice.h"
#include <iostream>

using namespace std;

LinuxKeyboardInputDevice::LinuxKeyboardInputDevice()
{
	
}

LinuxKeyboardInputDevice::~LinuxKeyboardInputDevice()
{
	
}

bool LinuxKeyboardInputDevice::isKeyDown(int key)
{
	if(FL_Enter == key)
		return this->enterKeyDown;
	else if((int) 'a' == key)
		return this->aKeyDown;

	return false;
}

void LinuxKeyboardInputDevice::keyDown(int event) {
	if(FL_Enter == Fl::event_key()) {
		this->enterKeyDown = true;
		cout << "ENTER KEY DOWN" << std::endl;
	} else if('a' == Fl::event_key()) {
		this->aKeyDown = true;
	}
}

void LinuxKeyboardInputDevice::keyUp(int event) {
	if(FL_Enter == Fl::event_key()) {
		this->enterKeyDown = false;
		cout << "ENTER KEY UP" << std::endl;
	} else if((int) 'a' == Fl::event_key()) {
		this->aKeyDown = false;
	}
}

