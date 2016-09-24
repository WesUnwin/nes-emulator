#ifndef LINUX_KEYBOARDINPUTDEVICE
#define LINUX_KEYBOARDINPUTDEVICE

#include "../../core/input/HostInputDevice.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>

class LinuxKeyboardInputDevice
{

public:
	LinuxKeyboardInputDevice();
	~LinuxKeyboardInputDevice();

	bool isKeyDown(int key);	


	void keyDown(int event);
	void keyUp(int event);
private:
	bool enterKeyDown;
	bool aKeyDown;
};


#endif