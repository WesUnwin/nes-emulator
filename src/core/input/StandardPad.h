#ifndef STANDARDPAD_H
#define STANDARDPAD_H

#include <string>
#include "NESInputDevice.h"


class StandardPad : public NESInputDevice
{

public:
	StandardPad();
	~StandardPad();

	virtual const std::string& getDeviceDescription();

	virtual int getNumberOfControls();
	virtual InputControl* getControl(int index);

	virtual void setStrobe(bool s);
	virtual void clock();
	virtual unsigned char read();

private:
	const std::string& description;
	ButtonControl* buttons[8];
	unsigned char shiftRegister;
	bool latch;
};



#endif