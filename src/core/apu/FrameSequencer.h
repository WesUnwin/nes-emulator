#ifndef APU_FRAMESEQUENCER_H
#define APU_FRAMESEQUENCER_H

#include "SquareWaveGenerator.h"
#include "TriangleWaveGenerator.h"
#include "NoiseChannel.h"
#include "DMChannel.h"


class FrameSequencer
{
public:
	FrameSequencer(SquareWaveGenerator* sq1, SquareWaveGenerator* sq2, TriangleWaveGenerator* t, NoiseChannel* n, DMChannel* d);
	~FrameSequencer();

	void advanceToNextStep();

private:
	int step;
	int mode;

	//APU's Channels:
	SquareWaveGenerator* square1;
	SquareWaveGenerator* square2;
	TriangleWaveGenerator* triangle;
	NoiseChannel* noise;
	DMChannel* dmc;

	void clockEnvelopesAndLinear();
	void clockLengthsAndSweeps();
};



#endif