#include "FrameSequencer.h"


FrameSequencer::FrameSequencer(SquareWaveGenerator* sq1, SquareWaveGenerator* sq2, TriangleWaveGenerator* t, NoiseChannel* n, DMChannel* d)
{
	square1 = sq1;
	square2 = sq2;
	triangle = t;
	noise = n;
	dmc = d;
}


FrameSequencer::~FrameSequencer()
{
}


void FrameSequencer::advanceToNextStep()
{
	//Increment Frame Sequence Step
	step++;
	if (((mode == 0) && (step >= 4)) || ((mode != 0) && (step >= 5)))
		step = 0;

	switch (step)
	{
	case 0:
		clockEnvelopesAndLinear();
		break;
	case 1:
		clockEnvelopesAndLinear();
		clockLengthsAndSweeps();
		break;
	case 2:
		clockEnvelopesAndLinear();
		break;
	case 3:

		break;
	case 4:

		break;
	}
}



void FrameSequencer::clockEnvelopesAndLinear()
{
	square1->clockEnvelope();
	square2->clockEnvelope();
	noise->clockEnvelope();
	triangle->clockLinearCounter();
}


void FrameSequencer::clockLengthsAndSweeps()
{
	square1->clockLengthAndSweep();
	square2->clockLengthAndSweep();
	triangle->clockLengthCounter();
	noise->clockLengthCounter();
}