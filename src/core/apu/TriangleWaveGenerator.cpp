#include "TriangleWaveGenerator.h"


const int TriangleWaveGenerator::LENGTHLOOKUP[32] =
{
	0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06,
	0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0xA1, 0x0E,
	0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16,
	0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E
};


const int TriangleWaveGenerator::SEQUENCE[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                                                 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};


TriangleWaveGenerator::TriangleWaveGenerator()
{
	sequence_pos = 0;
}


TriangleWaveGenerator::~TriangleWaveGenerator()
{
}


void TriangleWaveGenerator::reset()
{

}


void TriangleWaveGenerator::setChannelEnable(bool enableChannel)
{

}


int TriangleWaveGenerator::getLengthStatus()
{
	return (lengthCounter != 0) ? 1 : 0;
}


unsigned char TriangleWaveGenerator::readRegister(int regNum)
{
	return 0;
}


void TriangleWaveGenerator::writeToRegister(int regNum, unsigned char value)
{
	int index;
	switch (regNum)
	{
	case 0:  //$4008: clll lllll
		halt = (value & 0x80) ? true : false;
		linearCounterReloadValue = (value & 0x7F);
		break;
	case 1:

		break;
	case 2:  //$400A: pppp pppp
		timerPeriod &= 0x700;
		timerPeriod |= value;
		break;
	case 3:  //$400B: LLLL Lppp
		timerPeriod &= 0x0FF;
		timerPeriod |= (((int)(value & 7)) << 8);

		//Load Length Counter with Specified Duration (if not halted)
		if (lengthCounterEnable)
		{
			index = (value >> 3); //Forms a 5-bit index into length lookup table
			lengthCounter = LENGTHLOOKUP[index];
		}
		break;
	}
}


void TriangleWaveGenerator::clock(int cpuClocks)
{
	int clocksLeft = cpuClocks;

	while (clocksLeft > 0)
	{
		if (timer == 0)
		{
			//Timer Outputs a Clock : (clock sequencer if linear counter and length counters both contain non-zero values)
			if ((lengthCounter != 0) && (linearCounter != 0))  //If either linear counter or length counter is zero, sequencer is not clocked
			{
				//Clock Sequencer
				sequence_pos++;
				if (sequence_pos >= 32) sequence_pos = 0;

				clocksLeft--;
			}

			timer = timerPeriod;
		}
		else
			timer--;
	}
}


int TriangleWaveGenerator::getOutput()
{
	if ((lengthCounter == 0) || (linearCounter == 0))
		return 0;
	else
		return SEQUENCE[sequence_pos];
}


void TriangleWaveGenerator::clockLinearCounter()
{
	if (reloadLinearCounter) //Reload linear counter
	{
		linearCounter = linearCounterReloadValue;
	}
	else
	{
		if (linearCounter > 0) 
			linearCounter--;
	}

	if (halt == false) reloadLinearCounter = false;
}


void TriangleWaveGenerator::clockLengthCounter()
{
	//Clock Length Counter
	if ((!halt) && (lengthCounter > 0))
		lengthCounter--;
}