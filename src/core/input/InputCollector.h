#ifndef INPUT_INPUTCOLLECTOR_H
#define INPUT_INPUTCOLLECTOR_H

#include "NESInputDevice.h"

/**
* An abstract class defining the structure of an Input Collector; a class that implements the
* reponsibility of looking at the current state of the host keyboard/mouse and setting the state
* of the various controls (buttons, etc) on the current input devices being emulated.
*
* A platform-specific subclass of this class should be created, to encapsulate the functionality
* of binding to host keys and getting the state of the host keys.
*/
class InputCollector
{
public:
	InputCollector();
	~InputCollector();

	/**
	* The platform-specific subclass should implement this method, and iterate throught the controls
	* of the current input device for player 1 and 2, and based on the state of the host keyboard/mouse etc
	* set the state of each control appropriately.
	*/
	virtual void collectPlayerInput(NESInputDevice* player1Device, NESInputDevice* player2Device) = 0;
};

#endif