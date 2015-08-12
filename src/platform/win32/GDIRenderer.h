#ifndef GDIRENDERER_H
#define GDIRENDERER_H

#include <Windows.h>

#include "../../core/ppu/Renderer.h"
#include "../../core/ppu/DefaultMasterPalette.h"

struct BITMAPINFO_256
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD colors[256];
};

class GDIRenderer : public Renderer
{
public:
	GDIRenderer();
	~GDIRenderer();

	virtual void newFrame();
	virtual void drawPixels(int scanline, int startPixel, int endingPixel, unsigned char *scanlineBuffer);
	virtual void drawFrame();

	virtual void setMasterPalette(PPUCore::MASTERPALENTRY* colors);

	HDC getFrameBufferHDC();
private:
	int currentFrame; //Indicates wether Frame Buffer 1 or 2 is currently being drawn to  (the other frame buffer stores the last completed frame)
	//Frame Buffer 1 Variables
	HDC frameBuffer1HDC;
	HBITMAP frameBuffer1HBitmap;
	BITMAPINFO_256 frameBuffer1BMI;
	void* frameBuffer1Bits;

	//Frame Buffer 2 Variables
	HDC frameBuffer2HDC;
	HBITMAP frameBuffer2HBitmap;
	BITMAPINFO_256 frameBuffer2BMI;

	void* frameBuffer2Bits;

	unsigned char* currentPixelPointer;
};



#endif