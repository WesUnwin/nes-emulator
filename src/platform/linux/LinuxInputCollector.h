#ifndef LINUX_INPUT_PLAYER_H
#define LINUX_INPUT_PLAYER_H


#include "../../core/input/InputCollector.h"


class LinuxInputCollector : public InputCollector
{
public:
	LinuxInputCollector();
	~LinuxInputCollector();

	void keydown(int fl_event);
	void keyup(int fl_event);

	virtual void collectPlayerInput(NESInputDevice* player1Device, NESInputDevice* player2Device);
private:
	bool start;
};



#endif
