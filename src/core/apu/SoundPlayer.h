#ifndef APU_SOUNDPLAYER_H
#define APU_SOUNDPLAYER_H


/**
* Abstract class defining the required functionality of a Sound Player; a class responsible
* for sampling the state of the audio hardware periodically throughout a frame, then sending
* the block of samples to the underlying, platform specific OS/hardware for playback.
* 
* Implementation of this emulator on a new platform requires the development of a sub-class of
* this abstract class.
*/
class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	virtual int getCPUClocksPerSample() = 0;

	virtual void startNewFrame() = 0;

	virtual void recordSample(int sq1DAC, int sq2DAC, int triDAC, int noiseDAC, int dmcDAC) = 0;
};


#endif
