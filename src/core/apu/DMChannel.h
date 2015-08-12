#ifndef APU_DMCHANNEL_H
#define APU_DMCHANNEL_H


#include "AudioChannel.h"


class DMChannel : AudioChannel
{
public:
	DMChannel();
	~DMChannel();

	void reset();

	void setChannelEnable(bool enableChannel);
	int getLengthStatus();

	unsigned char readRegister(int regNum);
	void writeToRegister(int regNum, unsigned char value);

	void clock(int cpuClocks);
	int getOutput();
	

	bool getIRQStatus();


private:

	void clockOutputUnit();
	void fetchNextSampleByte();

	bool IRQEnabled;
	bool IRQFlag;

	bool loop;

	int bytesRemaining;

	unsigned short currentCPUAddress;

	unsigned char sampleBuffer;
	bool sampleBufferEmpty;

	unsigned char shiftRegister;
	int shiftRegister_BitsRemaining;

	bool silenceFlag;

	unsigned char outputLevel;

};



#endif
