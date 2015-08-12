#ifndef NES_H
#define NES_H


#include "resource.h"

#include "..\..\core\Emu.h"
#include "MainDialog.h"
#include "GDIRenderer.h"

#include "..\..\core\apu\SoundPlayer.h"
#include "WaveOutSoundPlayer.h"

#include "RomLoader.h"
#include "..\..\core\ppu\Renderer.h"
#include "Win32InputCollector.h"
#include "..\..\core\input\StandardPad.h"

class MainWindow;

extern MainWindow* mainWindow;
extern HINSTANCE hInst;
extern Emu* emu;
extern GDIRenderer* gdiRenderer;


#endif