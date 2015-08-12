#pragma once

#include <Windows.h>
#include "../../core/input/InputCollector.h"


class Win32InputCollector : public InputCollector
{
public:
	Win32InputCollector();
	~Win32InputCollector();


	virtual void collectPlayerInput(InputDevice* player1Device, InputDevice* player2Device);
};

