#include "SquareWaveGenerator.h"


const bool SquareWaveGenerator::DUTY_PATTERNS[4][8] =
{ 
{ false, true, false, false, false, false, false, false },  //Duty 0  (12.5%)
{ false, true, true, false, false, false, false, false },   //Duty 1  (25%)
{ false, true, true, true, true, false, false, false },     //Duty 2  (50%)
{ true, false, false, true, true, true, true, true }        //Duty 3  (75%)
};


const int SquareWaveGenerator::LENGTHLOOKUP[32] =
{
	0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 
	0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0xA1, 0x0E,
	0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16,
	0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E
};


SquareWaveGenerator::SquareWaveGenerator(bool secondary)
{
	this->secondary = secondary;
	reset();
}


SquareWaveGenerator::~SquareWaveGenerator()
{
}


void SquareWaveGenerator::reset()
{
	setupSequence(0xF, 0);
	lengthCounterEnable = false;
}


void SquareWaveGenerator::setChannelEnable(bool enableChannel)
{
	lengthCounterEnable = enableChannel;
	if (lengthCounterEnable == false)
		lengthCounter = 0;
}


int SquareWaveGenerator::getLengthStatus()
{
	if (lengthCounter == 0) return 0;
	else return 1;
}


unsigned char SquareWaveGenerator::readRegister(int regNum)
{
	unsigned char result = 0x40;  //Apparently reads turn 0x40 due to "heavy capacitance on the data bus"
	return result;
}


void SquareWaveGenerator::writeToRegister(int regNum, unsigned char value)
{
	int index;

	switch (regNum)
	{
	case 0:    //ddLE nnnn   (duty, env loop/disable length, env disable, vol/env)
		env_vol = (value & 0x0F); //Volumne (if env disabled) or decay rate

		env_disable = (value & 16) ? true : false;

		env_loop = (value & 32) ? true : false;
		halt = (value & 32) ? true : false;
		if (halt) lengthCounter = 0;

		sequence_duty = (value >> 6);	//Set Duty
		setupSequence( env_disable ? env_vol : env_counter, sequence_duty);

		break;
	case 1:    //Eppp nsss   (Sweep Control)
		sweep_enable = (value & 128) ? true : false;
		sweep_period = ((value >> 4) & 0x07);
		sweep_shift = (value & 0x07);
		sweep_negate = (value & 8) ? true : false;
		break;
	case 2:    //pppp pppp   (low bits of timer period)
		timer_period &= 0xF00;
		timer_period |= value;
		break;
	case 3:    //LLLL Lppp   (length index, high bits of timer period)
		timer_period &= 0x0FF;
		timer_period |= ((value & 0x7) << 8);

		//Load Length Counter with Specified Duration (if not halted)
		if (lengthCounterEnable)
		{
			index = (value >> 3); //Forms a 5-bit index into length lookup table
			lengthCounter = LENGTHLOOKUP[index];
		}
		break;
	default:
		break;
	}
}


void SquareWaveGenerator::clock(int cpuClocks)
{
	int clocksLeft = cpuClocks;  //# of !.79 MHz clocks till next sample reading

	while (clocksLeft > 0)
	{
		//Clock Timer
		if (timer_counter == 0)
		{
			//Timer outputs clock (after already effectively divided by 2): clock sequencer
			if (sequence_pos >= 7)
				sequence_pos = 0;
			else
				sequence_pos++;

			timer_counter = ((timer_period + 1) * 2) - 1;  //the * 2 takes care of the divide by 2 on the timer's output
		}
		else
			timer_counter--;

		clocksLeft--;
	}
	
}


int SquareWaveGenerator::getOutput()
{
	//Read current Amplitude output from DAC for SQ1
	int output = 0;
	if (lengthCounter != 0 && (sweep_silencechannel == false) && (timer_period > 7))
		output = sequence[sequence_pos];

	return output;
}


void SquareWaveGenerator::clockLengthAndSweep()
{
	//Clock Length Counter
	if ((!halt) && (lengthCounter > 0))
		lengthCounter--;

	//Clock Sweep Unit
	
	int newperiod;
	int delta = (timer_period >> sweep_shift);  //this is continuously calculated, may halt sweep unit
	if (sweep_negate == false)
		newperiod = timer_period + delta;
	else
		newperiod = timer_period - delta - (secondary ? 0 : 1); //if 1st square channel 1 more is subtracted (due to application of 1's complement subtraction)

	bool clockSweepDivider = true;
	if (timer_period < 8) clockSweepDivider = false;
	if (newperiod > 0x7FF) clockSweepDivider = false;

	sweep_silencechannel = !clockSweepDivider; //if not clocking sweep unit, channel is forced silent

	//Clock Sweep Divider/Counter
	if (clockSweepDivider)
	{
		if (sweep_counter == 0)
		{
			//Divider outputs a clocks:  Update Period
			if (sweep_enable && (sweep_shift != 0) && (lengthCounter != 0))
			{
				//Update the channel's period to the new value
				timer_period = (newperiod & 0x7FF);
			}

			sweep_counter = sweep_period; //Reset divider
		}
		else
			sweep_counter--;
	}
	

	//If write to sweep reg since last sweep unit clock, reset divider

}


void SquareWaveGenerator::clockEnvelope()
{
	if (env_reg3write)
	{
		env_counter = 15;
		env_divider = env_vol;  //Reset divider 

		env_reg3write = false;
	}
	else
	{
		//Clock divider
		if (env_divider == 0)
		{
			//Divider outputs clock, adjusting volume (env_counter)
			if (env_counter >= 0)
				env_counter--;
			else
			{
				if (env_loop)				
					env_counter = 0xF;
			}
			
			env_divider = env_vol;  //Refill divider
		}
		else
			env_divider--;
	}

	//If envelope enabled, the env_counter is directly controlling the channel's volume
	if (env_disable == false)
		setupSequence(env_counter, sequence_duty);

}





void SquareWaveGenerator::setupSequence(int vol, int duty)
{
	sequence_duty = duty;

	for (int i = 0; i < 8; i++)
		sequence[i] = DUTY_PATTERNS[duty][i] ? vol : 0;
}


