#ifndef LinuxSoundPlayer_H
#define LinuxSoundPlayer_H


#include "../../core/apu/SoundPlayer.h"


class LinuxSoundPlayer : SoundPlayer
{
public:
	LinuxSoundPlayer();
	~LinuxSoundPlayer();

	int getCPUClocksPerSample();

	void startNewFrame();

	void recordSample(int sq1DAC, int sq2DAC, int triDAC, int noiseDAC, int dmcDAC);
};


#endif
