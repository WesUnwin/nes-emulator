#include "iNesRom.h"


iNesRom::iNesRom(unsigned char* fileContents, unsigned int fileSizeInBytes)
{
	//Verify enough bytes for a full iNes header
	if (fileSizeInBytes < 16)
	{
		throw new iNesFileFormatException("File size too small - no iNes header");
	}

	//Verify NES signature
	if (fileContents[0] != 'N' || fileContents[1] != 'E' || fileContents[2] != 'S' || fileContents[3] != 0x1A)
	{
		throw new iNesFileFormatException("File contains no iNes signature - not a valid iNes ROM file");
	}

	//Allocate Memory For PRG ROM
	unsigned char prgRomBanks = fileContents[4];
	this->prgRomSize = prgRomBanks * 16384;

	unsigned char chrRomBanks = fileContents[5];
	this->chrRomSize = chrRomBanks * 8192;

	//NTSC or PAL Cartridge?
	this->PALCart = false;
	if (fileContents[9] & 0x01) this->PALCart = true;

	//Trainer present?
	this->hasTrainer = false;
	if (fileContents[6] & 4) this->hasTrainer = true;


	//Allocate Memory For Trainer and Load data
	if (this->hasTrainer)
	{
		this->trainer = new unsigned char[512];
		for (int i = 0; i < 512; i++)
			this->trainer[i] = fileContents[16 + i];
	}

	//Verify File Size is large enough to support contents included as claimed by header
	unsigned int requiredFileSize = 16 + (prgRomBanks * 16384);
	if (this->hasTrainer) requiredFileSize += 512;
	requiredFileSize += (chrRomBanks * 8192);
	if (fileSizeInBytes < requiredFileSize)
		throw new iNesFileFormatException("iNes ROM too small to contain the number of memory banks specified in the file header");

	//Allocate Memory for PRG ROM and load data
	if (prgRomBanks == 0)
		throw new iNesFileFormatException("iNes ROM indicates no PRG ROM banks are contained");
	else
	{
		this->prgRom = new unsigned char[this->prgRomSize];  //Allocate memory for PRG ROM
		
		unsigned int prgRomStartOffset = this->hasTrainer ? 16 + 512 : 16;

		for (unsigned int i = 0; i < prgRomSize; i++)
		{
			this->prgRom[i] = fileContents[prgRomStartOffset + i];
		}
	}

	//Allocate Memory for CHR ROM and load data
	if (chrRomBanks != 0)  // if 0, cart uses the 8KB of CHR RAM inside PPU
	{
		this->chrRom = new unsigned char[chrRomSize];

		unsigned int chrRomStartOffset = this->hasTrainer ? 16 + 512 : 16;
		chrRomStartOffset += (prgRomBanks * 16384);

		for (unsigned int i = 0; i < chrRomSize; i++)
		{
			this->chrRom[i] = fileContents[chrRomStartOffset + i];
		}
	}

	//Name Table Mirroring
	this->mirroring = NTMIRRORING_4NTVRAM;
	if (fileContents[6] & 8)
		this->mirroring = NTMIRRORING_4NTVRAM;
	else
	{
		if (fileContents[6] & 1)
			this->mirroring = NTMIRRORING_VERTICAL;
		else
			this->mirroring = NTMIRRORING_HORIZONTAL;
	}

	//Mapper
	this->mapperNumber = (fileContents[7] & 0xF0) + ((fileContents[6] >> 4) & 0x0F);
	constructMapper();  //based on this->mapperNumber creates CartMapper Subclass object

}


iNesRom::~iNesRom()
{
}




/**
* Based on this->mapperNumber, constructs a CartMapper subclass object for this iNesRom
* that will hold the state of the mapper hardware, and implement the mapper type's functionality.
*/
void iNesRom::constructMapper()
{
	this->mapper = MapperList::constructMapper(this->mapperNumber, this);
}


void iNesRom::setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu)
{
	this->mapper->setupMapper(cpu, ppu);
}


int iNesRom::getPRGROMSize()
{
	return this->prgRomSize;
}


unsigned char iNesRom::getPRGROMByte(int offset)
{
	if ((unsigned int)offset >= this->prgRomSize)
		return 0;
	else
		return this->prgRom[offset];
}


int iNesRom::getCHRROMSize()
{
	return this->chrRomSize;
}


unsigned char iNesRom::getCHRROMByte(int offset)
{
	if ((unsigned int)offset >= this->chrRomSize)
		return 0;
	else
		return this->chrRom[offset];
}


CartMapper* iNesRom::getMapper()
{
	return this->mapper;
}


//NTMIRRORRING iNesRom::getNTMirroring()
//{
//	return this->mirroring;
//}