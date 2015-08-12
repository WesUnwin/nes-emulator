#ifndef WIN32_WAVEOUTSOUNDPLAYER_H
#define WIN32_WAVEOUTSOUNDPLAYER_H


#include "../../core/apu/SoundPlayer.h"


class WaveOutSoundPlayer : SoundPlayer
{
public:
	WaveOutSoundPlayer();
	~WaveOutSoundPlayer();

	int getCPUClocksPerSample();

	void startNewFrame();

	void recordSample(int sq1DAC, int sq2DAC, int triDAC, int noiseDAC, int dmcDAC);
};


#endif