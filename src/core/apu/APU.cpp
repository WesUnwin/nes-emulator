#include "APU.h"


APU::APU(SoundPlayer* player)
{
	this->soundPlayer = player;

	this->square1 = new SquareWaveGenerator(false);
	this->square2 = new SquareWaveGenerator(true);
	this->triangle = new TriangleWaveGenerator();
	this->noise = new NoiseChannel();
	this->dmc = new DMChannel();

	this->frameSequencer = new FrameSequencer(square1, square2, triangle, noise, dmc);
}


APU::~APU()
{

}


void APU::newFrame()
{
	cpuClocksIntoFrame = 0;
	cpuClocksLeftToNextSample = soundPlayer->getCPUClocksPerSample();
	soundPlayer->startNewFrame();
}


void APU::finishFrame()
{

}


int APU::getCPUClocksIntoFrame()
{
	return cpuClocksIntoFrame;
}


void APU::advanceByCPUClocks(int cpuClocks)
{
	int clocksLeft = cpuClocks;
	
	while (clocksLeft > 0)
	{
		//Determine between frame seq advancing and sampling which will occur soonest (smallest # of cpuClocks into future)
		bool frameSeqAdvanceNearest = (cpuClocksTillFrameSeqAdvance <= cpuClocksLeftToNextSample) ? true : false;

		int clocksTillNextEvent = frameSeqAdvanceNearest ? cpuClocksTillFrameSeqAdvance : cpuClocksLeftToNextSample;

		if (clocksLeft >= clocksTillNextEvent)
		{
			clocksLeft -= clocksTillNextEvent;
			cpuClocksIntoFrame += clocksTillNextEvent;
			advanceChannels(clocksTillNextEvent);
			if (frameSeqAdvanceNearest)
			{
				frameSequencer->advanceToNextStep();
				cpuClocksTillFrameSeqAdvance = 7457;
			}
			else
				takeSample();
		}
		else
		{
			//Not enough cpu clocks to reach next sampling (or frame seq advance)
			advanceChannels(clocksLeft);
			cpuClocksIntoFrame += clocksLeft;
			clocksLeft = 0;
		}
	}
}


void APU::advanceChannels(int cpuClocks)
{
	square1->clock(cpuClocks);
	square2->clock(cpuClocks);
	triangle->clock(cpuClocks);
	noise->clock(cpuClocks);
	dmc->clock(cpuClocks);
}


void APU::takeSample()
{
	int s1 = square1->getOutput();
	int s2 = square2->getOutput();
	int t = triangle->getOutput();
	int n = noise->getOutput();
	int d = dmc->getOutput();

	soundPlayer->recordSample(s1, s2, t, n, d);
	cpuClocksLeftToNextSample = soundPlayer->getCPUClocksPerSample();
}


unsigned char APU::readPort(unsigned short cpuAddress)
{
	unsigned char result = 0;

	if ((cpuAddress < 0x4000) || (cpuAddress > 0x4017)) return 0;

	if (cpuAddress < 0x4004)
		square1->readRegister(cpuAddress & 3);
	else if (cpuAddress < 0x4008)
		square2->readRegister(cpuAddress & 3);
	else if (cpuAddress == 0x4015)
	{
		result |= square1->getLengthStatus();
		result |= (square2->getLengthStatus() << 1);
	}

	return result;
}


void APU::writeToPort(unsigned short cpuAddress, unsigned char value)
{
	if (cpuAddress == 0x4015)
	{
		square1->setChannelEnable(value & 1 ? true : false);  //Bit 0:  Disable/Enable Square 1 Channel
		square2->setChannelEnable(value & 2 ? true : false);  //Bit 1:  Disable/Enable Square 2 Channel
		triangle->setChannelEnable(value & 4 ? true : false); //Bit 2:  Disable/Enable Triangle Channel
		noise->setChannelEnable(value & 8 ? true : false);    //Bit 3:  Disable/Enable Noise Channel
		dmc->setChannelEnable(value & 16 ? true : false);     //Bit 4:  Disable/Enable DMC Channel
		//Bits 5-7:  (Unused)
	}
	else if (cpuAddress == 0x4017)
	{
		//Bit 7:  Frame Sequencer Mode (0: 4-step sequence, 1: 5-step sequence)
		//Bit 6:  0: Allow Frame Seq IRQs   1: Prevent Frame Seq IRQs
	}
}