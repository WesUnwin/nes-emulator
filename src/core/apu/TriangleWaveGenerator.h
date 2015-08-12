#ifndef APU_TRIANGLEWAVEGENERATOR_H
#define APU_TRIANGLEWAVEGENERATOR_H


#include "AudioChannel.h"


class TriangleWaveGenerator : AudioChannel
{
public:
	TriangleWaveGenerator();
	~TriangleWaveGenerator();

	void reset();

	void setChannelEnable(bool enableChannel);
	int getLengthStatus();

	unsigned char readRegister(int regNum);
	void writeToRegister(int regNum, unsigned char value);

	void clock(int cpuClocks);
	int getOutput();

	void clockLinearCounter();

	void clockLengthCounter();

private:
	int timer;
	int timerPeriod;

	int sequence_pos;
	static const int SEQUENCE[32];

	bool halt;
	int lengthCounter;
	bool lengthCounterEnable;

	static const int LENGTHLOOKUP[32];

	int linearCounter;
	int linearCounterReloadValue;
	bool reloadLinearCounter;

};




#endif