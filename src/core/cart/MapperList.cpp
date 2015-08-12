#include "MapperList.h"


MapperList::MapperList()
{
}


MapperList::~MapperList()
{
}


CartMapper* MapperList::constructMapper(int iNesMapperNum, iNesRom* rom)
{
	switch (iNesMapperNum)
	{
	case 0:
		return (CartMapper*) new Mapper0(rom);
	case 1:
		return (CartMapper*) new Mapper1(rom);
	default:
		throw new iNesFileFormatException("Mapper type not yet supported"); //All other mapper types not implemented yet
	}
}