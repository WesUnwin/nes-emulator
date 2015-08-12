#include "CartMapper.h"


CartMapper::CartMapper(iNesRom* rom)
{
	this->rom = rom;
}


CartMapper::~CartMapper()
{
}


unsigned char CartMapper::handleCartAccess(unsigned short cpuAddress, bool write, unsigned short writeValue)
{
	if (cpuAddress >= 0x8000)
	{
		//PRG-ROM Access
		if (write == false)
			return this->PRGROMBuffer[cpuAddress & 0x7FFF];
	}
	else if (cpuAddress >= 0x6000)
	{
		//Battery Backed Ram
	}

	return 0;
}