#ifndef LINUX_GLWINDOW_H
#define LINUX_GLWINDOW_H

#define GL_GLEXT_PROTOTYPES

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include "../../core/ppu/Renderer.h"
#include "GLRenderer.h"
#include "LinuxKeyboardInputDevice.h"

class GLWindow : public Fl_Gl_Window
{
	
	void draw();

	int handle(int event);

	double lasttime;

	int init;
	GLuint texID;


	GLuint PBOs[2];

	void setup();

	GLRenderer* glRenderer;
	LinuxKeyboardInputDevice* linuxKeyboardInputDevice;

public:
	GLWindow(GLRenderer* glRenderer, LinuxKeyboardInputDevice* linuxKeyboardInputDevice, int X, int Y, int W, int H);
};


class GLWindowRedrawListener : public RedrawListener
{
public:
	GLWindowRedrawListener(GLWindow* glWindow);
	~GLWindowRedrawListener();
	void onRedrawRequest();
private:
	GLWindow* glWindow;
};


#endif
