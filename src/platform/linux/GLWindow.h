#ifndef LINUX_GLWINDOW_H
#define LINUX_GLWINDOW_H

#define GL_GLEXT_PROTOTYPES

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include "../../core/ppu/Renderer.h"
#include "GLRenderer.h"


class GLWindow : public Fl_Gl_Window
{
	
	void draw();
	int handle(int e) 
	{
		switch (e) {
  			case FL_ENTER: cursor(FL_CURSOR_CROSS); break;
  			case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
  		}
  		return Fl_Gl_Window::handle(e);
	}
	double lasttime;

	int init;
	GLuint texID;


	GLuint PBOs[2];

	void setup();

	GLRenderer* glRenderer;
public:
	GLWindow(GLRenderer* glRenderer, int X, int Y, int W, int H);
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
