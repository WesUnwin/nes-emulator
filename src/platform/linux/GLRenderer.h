#ifndef GLRENDERER_H
#define GLRENDERER_H

#define GL_GLEXT_PROTOTYPES

#include <FL/gl.h>
#include <GL/gl.h>



#include "../../core/ppu/Renderer.h"
#include "../../core/ppu/DefaultMasterPalette.h"


class GLRenderer : public Renderer
{
public:
	GLRenderer();
	virtual ~GLRenderer();

	virtual void newFrame();
	virtual void drawPixels(int scanline, int startPixel, int endingPixel, unsigned char *scanlineBuffer);
	virtual void drawFrame();

	virtual void setMasterPalette(PPUCore::MASTERPALENTRY* colors);

	void* getCompletedFramePixels();

private:
	unsigned int masterPal[256];
	int currentFrame;
	unsigned int frameBuffer1[256 * 240];
	unsigned int frameBuffer2[256 * 240];
	unsigned int* currentPixelPointer;


};



#endif
