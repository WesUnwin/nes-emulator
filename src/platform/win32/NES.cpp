// NES.cpp : Defines the entry point for the application.
//


#include "NES.h"


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>



#define MAX_LOADSTRING 100

// Global Variables:
MainWindow* mainWindow;
HINSTANCE hInst;
Emu* emu;
GDIRenderer* gdiRenderer;

// Forward declarations of functions included in this code module:




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	gdiRenderer = new GDIRenderer();
	
	emu = new Emu(gdiRenderer, (SoundPlayer*) new WaveOutSoundPlayer(), new Win32InputCollector());

	emu->setInputDevice(1, new StandardPad());

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	// Perform application initialization:
	hInst = hInstance; // Store instance handle in our global variable


	//Create Main Application Window
	mainWindow = new MainWindow();

	bool classRegistered = mainWindow->registerClass(hInstance);
	if (!classRegistered)
	{
		MessageBoxA(HWND_DESKTOP, "Failed to register window class for main window", "Error", MB_OK);
		return FALSE;
	}

	bool mainWindowCreated = mainWindow->createWindow(hInstance, nCmdShow);
	if (!mainWindowCreated)
	{
		MessageBoxA(HWND_DESKTOP, "Failed to create application main window", "Error", MB_OK);
		return FALSE;
	}
	
	mainWindow->setWindowSize(512, 480);

	//Load Accelerators
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NES));

	// Main message loop:
	bool exitApplication = false;
	DWORD currentTime = 0;
	DWORD lastFrameTime = 0;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == 0) //No message in queue
		{
			//No Messages
			currentTime = GetTickCount();
			if (currentTime - lastFrameTime > 14 || lastFrameTime > currentTime)
			{
				emu->doFrame();
				lastFrameTime = GetTickCount();
			}
		}
		else
		{
			//There is 1 or more messages in queue
			if (GetMessage(&msg, NULL, 0, 0) == 0) exitApplication = true;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	} while (exitApplication == false);

	return (int) msg.wParam;
}




