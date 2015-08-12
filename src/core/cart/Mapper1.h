#ifndef CART_MAPPER1_H
#define CART_MAPPER1_H

#include "iNesRom.h"
#include "CartMapper.h"
#include "../ppu/PPUState.h"

class Mapper1 : public CartMapper
{
public:
	Mapper1(iNesRom* rom);
	~Mapper1();

	virtual void setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu);

	virtual unsigned char handleCartAccess(unsigned short cpuAddress, bool write, unsigned short writeValue);

private:
	PPUCore::PPU* ppu; //will need to holder point to, for changing NT Mirroring and CHR Rom banks

	unsigned char shiftRegister; //only use lowest 5 bits

	void transferShiftRegToInternalReg(unsigned short cpuAddressFifthWrite);

	unsigned char controlReg;
	unsigned char PRGBankReg;
	unsigned char CHRBank0Reg;
	unsigned char CHRBank1Reg;

	void setNTMirroring();

	void setupPRGMapping();

	void setupCHRMapping();

	int lowerPRGBank;
	int upperPRGBank;
	int lowerCHRBank;
	int upperCHRBank;

	/**
	* Sets which 16KB bank (0-15) of the 256KB ROM chip is to be visible at CPU 
	* addresses: $8000-$BFFF if upperPRGSpace is false, or $C000-$FFF if
	* upperPRGSpace is true. This function is still useful to setup 32KB swapping
	**/
	void setPRGBank(bool upperPRGSpace, int bankNum);
		
	void setCHRBank(bool upperPatternTable, int bankNum);

};


#endif