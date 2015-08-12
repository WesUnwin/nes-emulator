#ifndef CART_CARTMAPPER_H
#define CART_CARTMAPPER_H

#include "..\CPU\CPU.h"
#include "..\PPU\PPU.h"
class iNesRom; // #include "CART_iNesRom.h"

//Abstract class containing general template and functionality behind a cartridge mapper.
//a cart mapper is responsible for setup and config of of memory read/write handlers to implement ROM banking
class CartMapper
{
public:
	CartMapper(iNesRom* rom);
	~CartMapper();

	virtual void setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu) = 0;

	virtual unsigned char handleCartAccess(unsigned short cpuAddress, bool write, unsigned short writeValue);

protected:
	iNesRom* rom;

	unsigned char PRGROMBuffer[32768];
	unsigned char CHRROMBuffer[8192];
};



#endif