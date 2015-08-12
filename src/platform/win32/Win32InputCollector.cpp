#include "Win32InputCollector.h"


Win32InputCollector::Win32InputCollector()
{
}


Win32InputCollector::~Win32InputCollector()
{
}



void Win32InputCollector::collectPlayerInput(InputDevice* player1Device, InputDevice* player2Device)
{
	if (player1Device != 0)
	{
		for (int i = 0; i < player1Device->getNumberOfControls(); i++)
		{
			ButtonControl* button = (ButtonControl*) player1Device->getControl(i);
			std::string controlName = button->getName();

			if (controlName == "A") button->setState((GetKeyState(0x5A) & 0x8000) ? true : false);  //Z -> A button
			if (controlName == "B") button->setState((GetKeyState(0x58) & 0x8000) ? true : false);  //X -> B button
			if (controlName == "Select") button->setState((GetKeyState(VK_RSHIFT) & 0x8000) ? true : false);  //R Shift -> Select button
			if (controlName == "Start") button->setState((GetKeyState(VK_RETURN) & 0x8000) ? true : false);  //Enter -> Start button
			if (controlName == "Up") button->setState((GetKeyState(VK_UP) & 0x8000) ? true : false);  
			if (controlName == "Down") button->setState((GetKeyState(VK_DOWN) & 0x8000) ? true : false);  
			if (controlName == "Left") button->setState((GetKeyState(VK_LEFT) & 0x8000) ? true : false);  
			if (controlName == "Right") button->setState((GetKeyState(VK_RIGHT) & 0x8000) ? true : false); 
			
		}
	}
}