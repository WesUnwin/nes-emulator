#include "DMChannel.h"


DMChannel::DMChannel()
{
}


DMChannel::~DMChannel()
{
}


void DMChannel::reset()
{

}


void DMChannel::setChannelEnable(bool enableChannel)
{

}


int DMChannel::getLengthStatus()
{
	return (bytesRemaining != 0) ? 1 : 0;
}


unsigned char DMChannel::readRegister(int regNum)
{
	return 0;
}


void DMChannel::writeToRegister(int regNum, unsigned char value)
{

}


void DMChannel::clock(int cpuClocks)
{

}


int DMChannel::getOutput()
{
	return outputLevel;
}




void DMChannel::clockOutputUnit()  //called on every output clock of timer
{
	if (silenceFlag == false)
	{
		//Change Output Based on lowest bit of shift register
		if (shiftRegister & 1)
		{
			if (outputLevel <= 125)	outputLevel += 2;
		}
		else
		{
			if (outputLevel >= 2)
				outputLevel -= 2; //Raise output level by 2
		}
	}

	shiftRegister = (shiftRegister >> 1);

	shiftRegister_BitsRemaining--;
	if (shiftRegister_BitsRemaining <= 0)
	{
		//New output cycle (of 8 bits)
		shiftRegister_BitsRemaining = 8;
		if (sampleBufferEmpty)
			silenceFlag = true;
		else
		{
			silenceFlag = false;
			shiftRegister = sampleBuffer; //Sample buffer loaded into shift register
			sampleBufferEmpty = true;
			if (bytesRemaining != 0)
				fetchNextSampleByte();
		}
	}
			
}


void DMChannel::fetchNextSampleByte()
{
	//Read Byte at current CPU address into sample buffer (CPU halted for up to 4 clocks)

	//Increment CPU Address
	if (currentCPUAddress == 0xFFFF)
		currentCPUAddress = 0x8000;
	else
		currentCPUAddress++;

	//Decrement Bytes Remaining
	bytesRemaining--;
	if (bytesRemaining <= 0)
	{
		if (loop)
		{
			//Restart Playback

		}
		else
		{
			bytesRemaining = 0;
			if (IRQEnabled) IRQFlag = true; //Done, raise IRQ if IRQs enabled
		}
	}

}

bool DMChannel::getIRQStatus()
{
	return IRQFlag;
}
