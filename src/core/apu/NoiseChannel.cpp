#include "NoiseChannel.h"


const int NoiseChannel::PERIOD_LOOKUP[16] = 
{ 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 }; //These are Periods - not max counter values (for NTSC) 


const int NoiseChannel::LENGTHLOOKUP[32] =
{
	0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06,
	0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0xA1, 0x0E,
	0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16,
	0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E
};


NoiseChannel::NoiseChannel()
{
	reset();
}


NoiseChannel::~NoiseChannel()
{
}


void NoiseChannel::reset()
{
	shiftRegister = 1; //Powers up with initial value of 1
	lengthCounterEnable = false;
	lengthCounter = 0;
}


void NoiseChannel::setChannelEnable(bool enableLengthCounter)
{
	lengthCounterEnable = enableLengthCounter;
	if (lengthCounterEnable == false)
		lengthCounter = 0;
}


int NoiseChannel::getLengthStatus()
{
	return (lengthCounter != 0) ? 1 : 0;
}


unsigned char NoiseChannel::readRegister(int regNum)
{
	return 0;
}


void NoiseChannel::writeToRegister(int regNum, unsigned char value)
{
	switch (regNum)
	{
	case 0:  //$400C    --HC VVVV
		halt = (value & 32) ? true : false;
		constantVolume = (value & 16) ? true : false;
		envelopePeriod = value & 0x0F;
		if (constantVolume)
			vol = envelopePeriod;
		else
			vol = envelopeCounter;
	case 1:  //$400D    (Unused)
		break;
	case 2:  //$400E    M--- PPPP
		mode = (value >> 7);
		timerPeriod = PERIOD_LOOKUP[value & 0x0F] - 1;
		break;
	case 3:  //$400F    llll l---
		reloadEnvelopeDivider = true;

		//Load Length Counter with Specified Duration (if not halted)
		if (lengthCounterEnable)
			lengthCounter = LENGTHLOOKUP[value >> 3]; //Upper 5 bits of value form an index into a lookup table
	}
}


void NoiseChannel::clock(int cpuClocks)
{
	if (timer <= 0)
	{
		//Timer output, clocks linear feedback shift register
		int bit0 = (shiftRegister & 1);
		int otherBit = (mode == 0 ? ((shiftRegister >> 1) & 1) : ((shiftRegister >> 6) & 1));  //Mode 0: use bit 1, Mode 1: use bit 6
		int feedback = bit0 ^ otherBit;

		shiftRegister >>= 1;

		if (feedback) shiftRegister |= 0x4000;  //Set bit 14 to feedback value

		//Reload Timer
		timer = timerPeriod;
	}
	else
		timer--;	
}


int NoiseChannel::getOutput()
{
	//Output value (0-15) is the envelope volume if shiftRegister' bit 0 is clear 
	//and lengthCounter != 0, elsewise output is 0

	if ((lengthCounter != 0) && ((shiftRegister & 1) == 0))
		return vol;
	else
		return 0;
}


void NoiseChannel::clockLengthCounter()
{
	//Clock Length Counter
	if ((!halt) && (lengthCounter > 0))
		lengthCounter--;
}


void NoiseChannel::clockEnvelope()
{
	if (reloadEnvelopeDivider == false)
	{
		if (envelopeDivider == 0)
		{
			if (envelopeCounter > 0)
				envelopeCounter--;
			else if (halt) //this bit also enables envelope looping
				envelopeCounter = 15;
		}
		else
			envelopeDivider--;
	}
	else
	{
		reloadEnvelopeDivider = false;
		envelopeCounter = 15;
		envelopeDivider = envelopePeriod;
	}


	if (constantVolume)
		vol = envelopePeriod;
	else
		vol = envelopeCounter;
}