#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/gl.h>

#include "../../core/Emu.h"
#include "../../core/apu/SoundPlayer.h"
#include "../../core/cart/iNesRom.h"
#include "../../core/input/Control.h"
#include "../../core/input/StandardPad.h"
#include "../../core/input/HostInputDevice.h"

#include "GLWindow.h"
#include "GLRenderer.h"
#include "LinuxSoundPlayer.h"
#include "LinuxInputCollector.h"

#include <iostream>
#include <fstream>


Emu* emu;
GLRenderer* glRenderer;
LinuxInputCollector* linuxInputCollector;
LinuxKeyboardInputDevice* keyboard;


iNesRom* loadROMFromFile(char* filePath);
void rom_file_choosen(Fl_File_Chooser* fileChooser, void* d);


static void TimerCallback(void* userdata)
{
	((Emu*) userdata)->doFrame();
	Fl::repeat_timeout(1.0/60.0, TimerCallback, userdata);
}


void choose_rom()
{
	Fl_File_Chooser* fileChooser = new Fl_File_Chooser("/", "NES Rom File (*.nes)", Fl_File_Chooser::SINGLE, "Open File...");
	fileChooser->callback(rom_file_choosen);
	fileChooser->show();
}

void rom_file_choosen(Fl_File_Chooser* fileChooser, void* d)
{
	if(!fileChooser->visible())
	{
		iNesRom* rom = loadROMFromFile( (char*) fileChooser->value() );
		emu->loadROM(rom);
	}
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
	glRenderer = new GLRenderer();
	linuxInputCollector = new LinuxInputCollector();
	emu = new Emu(glRenderer, (SoundPlayer*) new LinuxSoundPlayer(), linuxInputCollector );

	// Create a Standard Pad (For Player 1) and configure it to use the host's keyboard
	keyboard = new LinuxKeyboardInputDevice();
	StandardPad* standardPad = new StandardPad();
	// for(int i = 0; i<standardPad->getNumberOfControls(); i++) {
	// 	InputControl* control = standardPad->getControl(i);
	// 	if(control->getName() == "A") {
	// 		control->setHostKeyBinding((HostInputDevice*) keyboard, (int) 'a');
	// 	} else if(control->getName() == "B") {

	// 	} else if(control->getName() == "Start") {
	// 		std::cout << "SETTING HOST KEY BINDING FOR STar" << std::endl;
	// 		control->setHostKeyBinding((HostInputDevice*) keyboard, FL_Enter);
	// 	} else if(control->getName() == "Select") {

	// 	}
	// }

	emu->setInputDevice(1, standardPad);

	Fl::gl_visual(FL_RGB);
	Fl_Window* window = new Fl_Window(512, 25 + 480, "NES Emulator");
	Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,512,25);		// Create menubar, items..
	menu->add("&File/&Open",  "^o", (Fl_Callback*) choose_rom);

	GLWindow* glWindow = new GLWindow(glRenderer, keyboard, 0, 25, 512, 480);

	window->resizable(glWindow);



	window->end();
	window->show(argc, argv);

	Fl::add_timeout(1.0 / 60.0, TimerCallback, (void*) emu );
	return Fl::run();
} 
