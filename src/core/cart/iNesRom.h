#ifndef CART_INESROM_H
#define CART_INESROM_H

#include "CartMapper.h"
#include "../cpu/CPU.h"
#include "../ppu/PPU.h"

#include "MapperList.h"

#include "iNesFileFormatException.h"


class iNesRom
{
public:
	iNesRom(unsigned char* fileContents, unsigned int fileSizeInBytes);
	~iNesRom();

	void setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu);

	enum NTMIRRORRING { NTMIRRORING_HORIZONTAL, NTMIRRORING_VERTICAL, NTMIRRORING_4NTVRAM };

	int getPRGROMSize();
	unsigned char getPRGROMByte(int offset);

	int getCHRROMSize();
	unsigned char getCHRROMByte(int offset);

	CartMapper* getMapper();

	NTMIRRORRING getNTMirroring() const {
		return mirroring;
	}

private:
	int mapperNumber;
	CartMapper* mapper;

	NTMIRRORRING mirroring;

	bool PALCart;  //True: Cart is designed for a pal system, false: NTSC Cartridge

	bool hasTrainer; 
	unsigned char* trainer;

	unsigned char* prgRom;
	unsigned char* chrRom;

	unsigned int prgRomSize; //in bytes
	unsigned int chrRomSize;

	void constructMapper();
};


#endif
