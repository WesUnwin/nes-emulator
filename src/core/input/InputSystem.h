#ifndef INPUT_INPUTSYSTEM_H
#define INPUT_INPUTSYSTEM_H

#include "NESInputDevice.h"
#include "InputCollector.h"


class InputSystem
{
public:
	InputSystem(InputCollector* inputCollector);
	~InputSystem();

	void setInputDevice(int player, NESInputDevice* inputDevice);

	void collectPlayerInput();

	void onPort4016Write(unsigned char value);
	unsigned char onPort4016Read();

	void onPort4017Write(unsigned char value);
	unsigned char onPort4017Read();

private:
	InputCollector* inputCollector;
	NESInputDevice* player1InputDevice;
	NESInputDevice* player2InputDevice;
};


#endif