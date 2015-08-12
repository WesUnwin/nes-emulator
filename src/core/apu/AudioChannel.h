#ifndef AUDIO_AUDIOCHANNEL_H
#define AUDIO_AUDIOCHANNEL_H


/**
* Abstract class representing the commonality amongst each of the 5 audio hardware channels in
* the Audio Processor Unit (APU).
*/
class AudioChannel
{
public:
	AudioChannel();
	~AudioChannel();

	/**
	* Resets the channel to it's initial, powered-up state.
	*/
	virtual void reset() = 0;

	/**
	* Enables/Silences the audio channel. This is invoked on each audio channel whenever a write
	* to the $4015 register is made.
	*/
	virtual void setChannelEnable(bool enableChannel) = 0;

	/**
	* Queries the status of the length counter (or samples left counter for the DMC).
	*/
	virtual int getLengthStatus() = 0;

	/**
	*  Reads one of the four registers for the channel.
	*  @param regNum  The register number to read (0-3)
	*/
	virtual unsigned char readRegister(int regNum) = 0;

	/**
	*  Writes to one of the four registers for this square wave channel.
	*  @param regNum  The register number to write to (0-3)
	*  @param value   The 8-bit value to write to the register.
	*/
	virtual void writeToRegister(int regNum, unsigned char value) = 0;

	/**
	* Advances the channel by the specified number of cpu clocks.
	* Does not clock units such as envelopes/length counters/etc which
	* are clocked in between calls to clock() when necesary by the 
	* frame sequencer.
	*/
	virtual void clock(int cpuClocks) = 0;

	/**
	* obtains the current output value of the channel's DAC.
	*/
	virtual int getOutput() = 0;
};



#endif