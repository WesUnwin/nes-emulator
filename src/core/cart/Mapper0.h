#ifndef CART_MAPPER0_H
#define CART_MAPPER0_H

#include "CartMapper.h"

//#include "CART_iNesRom.h"

#include "iNesRom.h"
#include "../cpu/CPU.h"
#include "../ppu/PPU.h"
#include "../ppu/PPUState.h"





class Mapper0 : public CartMapper
{
public:
	Mapper0(iNesRom* rom);
	~Mapper0();


	void setupMapper(CPUCore::CPU* cpu, PPUCore::PPU* ppu);
};

#endif
