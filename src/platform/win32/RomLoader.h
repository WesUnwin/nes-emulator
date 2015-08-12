#ifndef ROMLOADER_H
#define ROMLOADER_H

#include "windows.h"
#include <commdlg.h>

#include "../../core/cart/iNesRom.h"


class RomLoader
{
public:
	RomLoader();
	~RomLoader();

	static iNesRom* chooseRomFileToLoad(HWND hWnd);

	static iNesRom* loadRomFromFile(char* romFilePath);
};


#endif