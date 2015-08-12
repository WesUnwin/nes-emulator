#ifndef APU_SQUAREWAVEGENERATOR_H
#define APU_SQUAREWAVEGENERATOR_H

#include "AudioChannel.h"


class SquareWaveGenerator : AudioChannel
{
public:
	SquareWaveGenerator(bool secondary);
	~SquareWaveGenerator();

	void reset();

	void setChannelEnable(bool enableChannel);
	int getLengthStatus();

	unsigned char readRegister(int regNum);
	void writeToRegister(int regNum, unsigned char value);

	void clock(int cpuClocks);  
	int getOutput();


	/**
	 *  Called by the frame sequencer a few times per frame to clock 
	 *  the length counter and sweep unit for this channel.
	 */
	void clockLengthAndSweep(); 

	/**
	*  Called by the frame sequencer a few times per frame,
	*  clocks the envelope unit for this channel, adjusting the volume 
	*  periodically.
	*/
	void clockEnvelope();


private:
	bool secondary; //if true, this is the 2nd of the two square wave generators in the APU

	//Envelope 
	bool env_disable;
	
	int env_vol;   
	int env_counter;
	int env_divider;
	bool env_loop;
	bool env_reg3write; //a write to reg 3 (4th reg) since the last time the env unit was clocked

	//Sweep Unit
	bool sweep_enable;
	int sweep_period;
	int sweep_counter;
	int sweep_shift;    //0-7
	bool sweep_negate;  //Controls direction of sweeping (whether to increase/decrease timer period)
	bool sweep_silencechannel;

	//Timer
	int timer_period;   //Hold's on to the current period (used to refill the timer divider)
	int timer_counter;    //Down-counter (divides the input 1.79 MHz clock, output clocks

	//Length Counter
	bool lengthCounterEnable;
	int lengthCounter;  //Down-counter
	bool halt;

	static const int LENGTHLOOKUP[32];

	
	//Sequence
	int sequence[8];  //The value at each index is either the envelope value (representing high)  or 0 (if low)
	int sequence_pos;
	int sequence_duty;


	void setupSequence(int vol, int duty);

	static const bool DUTY_PATTERNS[4][8];

};



#endif