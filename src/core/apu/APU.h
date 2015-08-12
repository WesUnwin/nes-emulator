#ifndef APU_APU_H
#define APU_APU_H

#include "SoundPlayer.h"

#include "FrameSequencer.h"

#include "SquareWaveGenerator.h"
#include "TriangleWaveGenerator.h"
#include "NoiseChannel.h"
#include "DMChannel.h"


class APU
{
public:
	APU(SoundPlayer* player);
	~APU();

	void newFrame();

	void advanceByCPUClocks(int cpuClocks);
	void finishFrame();

	int getCPUClocksIntoFrame();

	unsigned char readPort(unsigned short cpuAddress);
	void writeToPort(unsigned short cpuAddress, unsigned char value);

private:
	FrameSequencer* frameSequencer;

	void advanceChannels(int cpuClocks);
	void takeSample();

	int cpuClocksIntoFrame;

	int cpuClocksTillFrameSeqAdvance;

	SoundPlayer* soundPlayer;
	int cpuClocksLeftToNextSample;




	bool FrameSeq_Mode;   //O: 4-step sequence     1: 5-step sequence 
	int FrameSeq_Step;
	bool FrameSeq_Interrupt;

	SquareWaveGenerator* square1;
	SquareWaveGenerator* square2;
	TriangleWaveGenerator* triangle;
	NoiseChannel* noise;
	DMChannel* dmc;


	bool IRQDisabled;

};



#endif