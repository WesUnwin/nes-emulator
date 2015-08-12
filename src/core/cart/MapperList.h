#ifndef CART_MAPPERLIST_H
#define CART_MAPPERLIST_H

#include "iNesRom.h"
#include "iNesFileFormatException.h"
#include "CartMapper.h"

#include "Mapper0.h"
#include "Mapper1.h"


class MapperList
{
public:
	MapperList();
	~MapperList();

	static CartMapper* constructMapper(int iNesMapperNum, iNesRom* rom);
};



#endif