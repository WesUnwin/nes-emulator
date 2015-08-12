#include "Mapper0.h"


Mapper0::Mapper0(iNesRom* rom) : CartMapper(rom)
{
	//Super class constructor called first

	//Fill in PRG-ROM Read Buffer with PRG-ROM Banks in Rom
	if (rom->getPRGROMSize() ==  16384)
	{
		for (int i = 0; i < 16384; i++)
		{
			this->PRGROMBuffer[i] = rom->getPRGROMByte(i);
			this->PRGROMBuffer[i + 16384] = rom->getPRGROMByte(i);
		}
	}
	else
	{
		for (int i = 0; i < 32768; i++)
			this->PRGROMBuffer[i] = rom->getPRGROMByte(i);
	}

	//Load CHR-ROM Buffer
	int chrRomSize = rom->getCHRROMSize();
	if (chrRomSize > 8192) chrRomSize = 8192; //Mapper 0 can only support up to 8KB of CHR ROM

	for (int i = 0; i < chrRomSize; i++)
		this->CHRROMBuffer[i] = rom->getCHRROMByte(i);

}


Mapper0::~Mapper0()
{
}


void Mapper0::setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu)
{
	//Direct PPU to location of CHR ROM/RAM
	if (this->rom->getCHRROMSize() == 0)
		ppu->setCHRMemPointer(0); //Tell PPU to use internal 8KB of CHR Ram
	else
		ppu->setCHRMemPointer(this->CHRROMBuffer);

	//Set PPU NT Mirroring as desired by ROM file
	if (this->rom->getNTMirroring() == iNesRom::NTMIRRORRING::NTMIRRORING_HORIZONTAL)
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::HORIZONTAL);
	else if (this->rom->getNTMirroring() == iNesRom::NTMIRRORRING::NTMIRRORING_VERTICAL)
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::VERTICAL);
	else if (this->rom->getNTMirroring() == iNesRom::NTMIRRORRING::NTMIRRORING_4NTVRAM)
		ppu->setNTMirroring(PPUCore::PPUState::NTMIRRORING::FOURSCREEN);
}


