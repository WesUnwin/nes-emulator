#ifndef APU_NOISECHANNEL_H
#define APU_NOISECHANNEL_H


#include "AudioChannel.h"


class NoiseChannel : AudioChannel
{
public:
	NoiseChannel();
	~NoiseChannel();

	void reset();

	void setChannelEnable(bool enableLengthCounter);
	int getLengthStatus();

	unsigned char readRegister(int regNum);
	void writeToRegister(int regNum, unsigned char value);

	void clock(int cpuClocks);
	int getOutput();


	void clockLengthCounter();

	void clockEnvelope();

private:
	unsigned short shiftRegister;  //Only uses lowest 15 bits 

	int timer;
	int timerPeriod;  //actually period - 1
	int mode;

	bool halt; //also indicated to loop envelope if using envelope
	int lengthCounter;
	bool lengthCounterEnable;

	bool reloadEnvelopeDivider;
	bool constantVolume;
	int envelopePeriod;
	int envelopeDivider;
	int envelopeCounter; //Stores current volume when using envelope over constant volume
	int vol;

	static const int PERIOD_LOOKUP[16];

	static const int LENGTHLOOKUP[32];
};


#endif

