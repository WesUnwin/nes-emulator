#include "ScrollCounterOps.h"

//        SCROLL COUNTER DEFINITIONS:
//unsigned int BGScrollCountersLatch;     //VHYYYYYXXXXX
//unsigned int BGScrollFineVerticalLatch; //         yyy
//
//unsigned int BGScrollCounters;		  //VHYYYYYXXXXX
//unsigned int BGScrollFineHorizontal;    //xxx  (have no corresponding latches)
//
//unsigned int PAR;                       //SppppppppByyy


namespace PPUCore
{
	namespace ScrollCounterOps
	{
		void port0Write(PPUState *state, unsigned char value)
		{
			state->BGScrollCountersLatch &= 0xFFFFF3FF;
			state->BGScrollCountersLatch |= (((unsigned int)(value & 0x3) << 10)); //lower 2 bits of value set the nametable scroll bits (VH)

			state->PAR &= 0x00000FFF;
			if (value & 0x10) state->PAR |= 0x1000;  //Bit 4 of value sets the BG Pattern Table select bit (S)
		}


		void port5Write_0(PPUState *state, unsigned char value)
		{
			state->BGScrollFineHorizontal = (unsigned int)(value & 0x07);  //lowest 3 bits of value set fine horizontal scroll,

			state->BGScrollCountersLatch &= 0xFFFFFFE0;
			state->BGScrollCountersLatch += (unsigned int)(value >> 3);  //upper 5 bits of value set horizontal tile scroll
		}


		void port5Write_1(PPUState *state, unsigned char value)
		{
			state->BGScrollFineVerticalLatch = (value & 0x07); //lower 3 bits of value set the fine vertical scroll

			state->BGScrollCountersLatch &= 0xFFFFFC1F;
			state->BGScrollCountersLatch |= ((unsigned int)(value >> 3)) << 5; //upper 5 bits set the vertical tile scroll
		}


		void port6Write_0(PPUState *state, unsigned char value)
		{
			state->BGScrollCountersLatch &= 0x000000FF;  //Clear VH and upper 2 vertical tile scroll bits
			state->BGScrollCountersLatch |= (((unsigned int)(value & 0x03)) << 8);  //Lowest 2 bits of value set the upper 2 of the 5 bits of the vertical tile scroll

			state->BGScrollCountersLatch |= (((unsigned int)((value & 0x0C) >> 2)) << 10);  //Bits 2 and 3 of value set the vertical and horizontal name table bits (VH)

			state->BGScrollFineVerticalLatch = 0; //Clear ALL 3 bits
			state->BGScrollFineVerticalLatch |= ((unsigned int)((value & 0x30) >> 4));  //Bits 4 and 5 of value set the lower 2 bits of the fine vertical scroll

			//TODO is highest of the 3 fine scroll latch bits left untouched or set to zero maybe? (I believe set to 0 as per wiki.nesdev.com

			//Bits 6 and 7 of value are unused
		}


		void port6Write_1(PPUState *state, unsigned char value)
		{
			state->BGScrollCountersLatch &= 0xFFFFFFE0;
			state->BGScrollCountersLatch += (value & 0x1F);  //lowest 5 bits of value set horizontal tile scroll

			state->BGScrollCountersLatch &= 0xFFFFFF1F; //upper 3 bits of value set lower 3 bits of vertical tile scroll
			state->BGScrollCountersLatch |= ((unsigned int)(value & 0xE0));

			//All counters are then updated to their latched values
			state->BGScrollCounters = state->BGScrollCountersLatch;			
			state->PAR &= 0x1FF8; 
			state->PAR |= (state->BGScrollFineVerticalLatch & 0x7);

		}


		void loadLatchedHorizontalCounters(PPUState *state)
		{
			unsigned int latchedHCounters = (state->BGScrollCountersLatch & 0x041F);
			state->BGScrollCounters &= 0xBE0;  //Keep state of all vertical counters
			state->BGScrollCounters |= latchedHCounters;  //Set H counters to their latched values from BGScrollCountersLatch
		}


		void loadLatchedVerticalCounters(PPUState *state)
		{
			unsigned int latchedVCounters = (state->BGScrollCountersLatch & 0x0BE0);  //VHYYYYYXXXXX
			state->BGScrollCounters &= 0x041F; //Clear all bits except the H counters
			state->BGScrollCounters |= latchedVCounters; 

			state->PAR &= 0x1FF8;  
			state->PAR |= (state->BGScrollFineVerticalLatch & 0x7);  //Fine Vertical Scroll <= Latched Fine Vertical Scroll
		}


		void incrementVerticalCounters(PPUState* state)
		{
			//Daisy Chaining of vertical counters:    yyy -> YYYYY -> V   

			if ((state->PAR & 0x7) == 7) //Overflow of yyy
			{
				state->PAR &= 0xFFFFFFF8; //Clear yyy to 0

				//Increment YYYYY
				if ((state->BGScrollCounters & 0x3E0) == 0x3A0)  //YYYYY is at 29 (this counter overflows when going to 30, to prevent scrolling into the last 2 rows which are attribute bytes)
				{
					state->BGScrollCounters &= 0xFFFFFC1F;  //Clears YYYYY to 0
					state->BGScrollCounters ^= 0x800; //Advance vertically to next name table  (by toggling V bit)
				}
				else if ((state->BGScrollCounters & 0x3E0) == 0x3E0) //YYYY is at 31 (will improperly overflow on this increment)
				{
					state->BGScrollCounters &= 0xFFFFFC1F;  //Clears YYYYY to 0
					//NO vertical name table switch happens on this improper overflow
				}
				else
					state->BGScrollCounters += 32; //Increment YYYYY by 1   (note YYYYY is << 5 within BGScrollCounters
			}
			else
			{
				state->PAR++;
			}
		}


		void incrementHorizontalScrollCounters(PPUState* state)
		{
			if ((state->BGScrollCounters & 0x1F) == 31)
			{
				state->BGScrollCounters &= 0xFE0; //Clear HT
				state->BGScrollCounters ^= 0x400; //Increment H NT bit
			}
			else
				state->BGScrollCounters++;
		}


		void incrementScrollCountersBy1(PPUState *state)
		{
			int entireScrollCounters = ((state->PAR & 7) << 12) | (state->BGScrollCounters & 0xFFF);

			entireScrollCounters += 1;

			state->BGScrollCounters = (entireScrollCounters & 0xFFF);
			state->PAR &= 0x1FF8;
			state->PAR |= (entireScrollCounters >> 12);
		}


		void incrementScrollCountersBy32(PPUState *state)
		{
			int entireScrollCounters = ((state->PAR & 7) << 12) | (state->BGScrollCounters & 0xFFF);

			entireScrollCounters += 32;

			state->BGScrollCounters = (entireScrollCounters & 0xFFF);
			state->PAR &= 0x1FF8;
			state->PAR |= (entireScrollCounters >> 12);
		}
	}
}
