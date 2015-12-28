#include "GLWindow.h"
#include <iostream>


#define v3f(x) glVertex3fv(x)


using namespace std;


GLWindow::GLWindow(GLRenderer* glRenderer, int X, int Y, int W, int H) : Fl_Gl_Window(X,Y,W,H,0) {
	this->glRenderer = glRenderer;

	GLWindowRedrawListener* redrawListener = new GLWindowRedrawListener(this);
	glRenderer->setRedrawListener(redrawListener);

	lasttime = 0.0;
	init = 0;
	texID = 0;
}

void GLWindow::draw()
{
  //cout << "FRAME DRAWN " << w() << " " << h() << std::endl;


  if (this->init == 0) {
	  cout << "INIT PBO" << std::endl;

	  glGenTextures(1, &(this->texID));
	  glBindTexture(GL_TEXTURE_2D, this->texID);

	  glEnable(GL_TEXTURE_2D);
	  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//	  unsigned int* pixel_data = new unsigned int[50 * 50];
//	  for(int x = 0;  x<(50*50); x++)
//		  pixel_data[x] = 0xFFFF00FF;
//
//	  pixel_data[75] = 0x00FF0000;
//	  pixel_data[(50*50) - 1] = 0x0000FF00;

	  void* pixel_data = glRenderer->getCompletedFramePixels();

	  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256,240, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) pixel_data );

	  glBindTexture(GL_TEXTURE_2D, this->texID);




	  this->init = 1;
  }
  //glGenBuffers(1, PBOs);
//  glBindTexture(GL_TEXTURE_2D, textureId);
//
//  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBOs[index]);
//  glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, IMAGE_WIDTH, IMAGE_HEIGHT, PIXEL_FORMAT, GL_UNSIGNED_BYTE, 0);
//
//
//  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBOs[nextIndex]);
//  glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
//  GLubyte* ptr = (GLubyte*) glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

  glViewport(0,0, (GLsizei) w(), (GLsizei) h());

  lasttime = lasttime+1.0;


  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, w(), 0.0, h(), -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glColor3f(0.5, 0.0, 0.0);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
  glBindTexture(GL_TEXTURE_2D, this->texID);

  unsigned int* pixel_data = (unsigned int*) glRenderer->getCompletedFramePixels();

//  static int i = 1;
//
//  for(int x = 0;  x<(256*240); x++)
//	  pixel_data[x] = 0xFFFF00FF;
//
//  pixel_data[75] = 0x00FF0000;
//  pixel_data[(256*240) - i] = 0x0000FF00;

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256,240, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) pixel_data );

  //i++;

  glBegin(GL_QUADS);

  float width = (float) w();
  float height = (float) h();

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width, 0.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width, height, 0.0f);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0f, height, 0.0f);

  glEnd();


  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}


GLWindowRedrawListener::GLWindowRedrawListener(GLWindow* glWindow)
{
	this->glWindow = glWindow;
}


GLWindowRedrawListener::~GLWindowRedrawListener()
{

}


void GLWindowRedrawListener::onRedrawRequest()
{
	this->glWindow->redraw();
}
