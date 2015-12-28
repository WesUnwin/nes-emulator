#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/gl.h>

#include "../../core/Emu.h"
#include "../../core/apu/SoundPlayer.h"
#include "../../core/cart/iNesRom.h"

#include "GLWindow.h"
#include "GLRenderer.h"
#include "LinuxSoundPlayer.h"
#include "LinuxInputCollector.h"

#include <iostream>
#include <fstream>


Emu* emu;
GLRenderer* glRenderer;


iNesRom* loadROMFromFile(char* filePath);


static void TimerCallback(void* userdata)
{
	((Emu*) userdata)->doFrame();
	Fl::repeat_timeout(1.0/60.0, TimerCallback, userdata);
}


void choose_rom()
{
	std::cout << "CHOOSE A ROM" << std::endl;
	iNesRom* rom = loadROMFromFile("/home/user/Desktop/smb.nes");
	emu->loadROM(rom);
}


iNesRom* loadROMFromFile(char* filePath)
{
	iNesRom* rom = 0;

	std::ifstream romFile(filePath, std::ios::in | std::ios::binary | std::ios::ate);
	if( romFile.is_open() ) {
		int size = romFile.tellg();
		char* file_contents = new char[size];
		romFile.seekg(0, std::ios::beg);
		romFile.read(file_contents, size);
		romFile.close();

		rom = new iNesRom( (unsigned char*) file_contents, size);
	} else
		std::cout << "FAILED TO OPEN FILE" << std::endl;

	return rom;
}


int main(int argc, char **argv) 
{
	GLRenderer* glRenderer = new GLRenderer();
	emu = new Emu(glRenderer, (SoundPlayer*) new LinuxSoundPlayer(), new LinuxInputCollector());


	Fl::gl_visual(FL_RGB);
	Fl_Window* window = new Fl_Window(512, 25 + 480, "NES Emulator");
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,512,25);		// Create menubar, items..
	menu->add("&File/&Open",  "^o", (Fl_Callback*) choose_rom);

	GLWindow* glWindow = new GLWindow(glRenderer, 0, 25, 512, 480);

	window->resizable(glWindow);



	window->end();
	window->show(argc, argv);

	Fl::add_timeout(1.0 / 60.0, TimerCallback, (void*) emu );
	return Fl::run();
} 
