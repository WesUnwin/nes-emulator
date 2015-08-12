#ifndef INPUT_INPUTDEVICE_H
#define INPUT_INPUTDEVICE_H

#include <string>

#include "Control.h"


class InputDevice
{
public:
	InputDevice();
	~InputDevice();

	virtual const std::string& getDeviceDescription() = 0;

	virtual int getNumberOfControls() = 0;
	virtual InputControl* getControl(int index) = 0;
	
	virtual void setStrobe(bool s) = 0;
	virtual void clock() = 0;
	virtual unsigned char read() = 0;

};


#endif