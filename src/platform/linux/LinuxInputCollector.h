#ifndef LINUX_INPUT_PLAYER_H
#define LINUX_INPUT_PLAYER_H


#include "../../core/input/InputCollector.h"


class LinuxInputCollector : public InputCollector
{
public:
	LinuxInputCollector();
	~LinuxInputCollector();


	virtual void collectPlayerInput(InputDevice* player1Device, InputDevice* player2Device);
};



#endif
