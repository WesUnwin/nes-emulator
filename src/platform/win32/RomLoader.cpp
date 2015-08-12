#include "RomLoader.h"


RomLoader::RomLoader()
{
}


RomLoader::~RomLoader()
{
}


iNesRom* RomLoader::chooseRomFileToLoad(HWND hWnd)
{
	char szFile[256];

	szFile[0] = 0;

	OPENFILENAMEA openFileName;

	ZeroMemory(&openFileName, sizeof(OPENFILENAMEA));
	openFileName.lStructSize = sizeof OPENFILENAMEA;
	openFileName.hwndOwner = hWnd;
	openFileName.hInstance = GetModuleHandle(0);
	openFileName.lpstrFilter = (LPSTR) "All\0*.*\0Text\0*.TXT\0";
	openFileName.lpstrCustomFilter = 0;
	openFileName.nMaxCustFilter = 0;
	openFileName.nFilterIndex = 1;
	openFileName.lpstrFile = (LPSTR)szFile;
	openFileName.nMaxFile = sizeof(szFile);
	openFileName.lpstrFileTitle = (LPSTR) "Load Binary File...";
	openFileName.nMaxFileTitle = 0;
	openFileName.lpstrInitialDir = NULL;
	openFileName.lpstrTitle = 0;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	openFileName.nFileOffset = 0;
	openFileName.nFileExtension = 0;
	openFileName.lpstrDefExt = 0;
	openFileName.lCustData = 0;
	openFileName.lpfnHook = 0;
	openFileName.lpTemplateName = 0;

	iNesRom* rom = 0;

	if (GetOpenFileNameA(&openFileName) != 0)
	{
		rom = RomLoader::loadRomFromFile(szFile);
	}

	return rom;
}


iNesRom* RomLoader::loadRomFromFile(char* romFilePath)
{
	HANDLE hFile = 0;
	
	
	try
	{
		hFile = CreateFileA(romFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}
	catch (...)
	{
		MessageBoxA(0, "Unable to access ROM file", "Error loading ROM File", MB_OK);
		return NULL;
	}

	DWORD fileSizeUpper = 0;
	DWORD fileSize = GetFileSize(hFile, &fileSizeUpper);

	if (fileSizeUpper != 0 || fileSize > 10000000)
	{
		//File is too large for an iNes file
		MessageBoxA(0, "File is to large - ensure the selected file is an iNes ROM file (.nes)", "Unable to Load ROM File", MB_OK);
		return NULL;
	}

	DWORD bytesRead;
	unsigned char* fileContents = new unsigned char[fileSize];

	try
	{
		ReadFile(hFile, fileContents, fileSize, &bytesRead, NULL);
	}
	catch (...)
	{
		MessageBoxA(0, "ROM File could not be read.", "Error", MB_OK);
		return NULL;
	}

	iNesRom* rom = NULL;

	try
	{
		//Try to Create iNesRom object for file
		rom = new iNesRom(fileContents, fileSize);
	}
	catch (iNesFileFormatException* e)
	{
		//Formatting Error found in Rom
		MessageBoxA(0, e->what(), "Unable to load ROM File:", MB_OK);
		return NULL;
	}

	return rom;
}