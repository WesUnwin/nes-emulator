#include "GDIRenderer.h"



GDIRenderer::GDIRenderer()
{
	frameBuffer1BMI.bmiHeader.biSize = frameBuffer2BMI.bmiHeader.biSize = sizeof BITMAPINFOHEADER;
	frameBuffer1BMI.bmiHeader.biWidth = frameBuffer2BMI.bmiHeader.biWidth = 256;
	frameBuffer1BMI.bmiHeader.biHeight = frameBuffer2BMI.bmiHeader.biHeight = - 241;
	frameBuffer1BMI.bmiHeader.biBitCount = frameBuffer2BMI.bmiHeader.biBitCount = 8;
	frameBuffer1BMI.bmiHeader.biPlanes = frameBuffer2BMI.bmiHeader.biPlanes = 1;
	frameBuffer1BMI.bmiHeader.biCompression = frameBuffer2BMI.bmiHeader.biCompression = 0;
	frameBuffer1BMI.bmiHeader.biSizeImage = frameBuffer2BMI.bmiHeader.biSizeImage = 245760;   //256 x 240 x 4 bytes per pixel
	frameBuffer1BMI.bmiHeader.biXPelsPerMeter = frameBuffer2BMI.bmiHeader.biXPelsPerMeter = 0;
	frameBuffer1BMI.bmiHeader.biYPelsPerMeter = frameBuffer2BMI.bmiHeader.biYPelsPerMeter = 0;
	frameBuffer1BMI.bmiHeader.biClrUsed = frameBuffer2BMI.bmiHeader.biClrUsed = 0;
	frameBuffer1BMI.bmiHeader.biClrImportant = frameBuffer2BMI.bmiHeader.biClrImportant = 0;

	//Copy in master palette
	for (int i = 0; i < 64; i++)
	{
		frameBuffer1BMI.colors[i].rgbRed = PPUCore::DEFAULT_MASTER_PALETTE[i].Red;
		frameBuffer1BMI.colors[i].rgbGreen = PPUCore::DEFAULT_MASTER_PALETTE[i].Green;
		frameBuffer1BMI.colors[i].rgbBlue = PPUCore::DEFAULT_MASTER_PALETTE[i].Blue;

		frameBuffer2BMI.colors[i].rgbRed = PPUCore::DEFAULT_MASTER_PALETTE[i].Red;
		frameBuffer2BMI.colors[i].rgbGreen = PPUCore::DEFAULT_MASTER_PALETTE[i].Green;
		frameBuffer2BMI.colors[i].rgbBlue = PPUCore::DEFAULT_MASTER_PALETTE[i].Blue;
	}

	this->frameBuffer1HDC = CreateCompatibleDC(0);
	this->frameBuffer1HBitmap = CreateDIBSection(this->frameBuffer1HDC, (const BITMAPINFO*)&frameBuffer1BMI, DIB_RGB_COLORS, &frameBuffer1Bits, 0, 0);
	SelectObject(this->frameBuffer1HDC, this->frameBuffer1HBitmap);

	this->frameBuffer2HDC = CreateCompatibleDC(0);
	this->frameBuffer2HBitmap = CreateDIBSection(this->frameBuffer2HDC, (const BITMAPINFO*)&frameBuffer2BMI, DIB_RGB_COLORS, &frameBuffer2Bits, 0, 0);
	SelectObject(this->frameBuffer2HDC, this->frameBuffer2HBitmap);

	this->currentFrame = 0;
}


GDIRenderer::~GDIRenderer()
{

}


void GDIRenderer::setMasterPalette(PPUCore::MASTERPALENTRY* colors)
{
	for (int i = 0; i < 64; i++)
	{
		frameBuffer1BMI.colors[i].rgbRed = colors[i].Red;
		frameBuffer1BMI.colors[i].rgbGreen = colors[i].Green;
		frameBuffer1BMI.colors[i].rgbBlue = colors[i].Blue;

		frameBuffer2BMI.colors[i].rgbRed = colors[i].Red;
		frameBuffer2BMI.colors[i].rgbGreen = colors[i].Green;
		frameBuffer2BMI.colors[i].rgbBlue = colors[i].Blue;
	}
}

void GDIRenderer::newFrame()
{
	this->currentFrame = this->currentFrame == 0 ? 1 : 0;  //Toggle between buffers

	if (this->currentFrame == 0)
		currentPixelPointer = (unsigned char*) this->frameBuffer1Bits;
	else
		currentPixelPointer = (unsigned char*) this->frameBuffer2Bits;
}

void GDIRenderer::drawPixels(int scanline, int startPixel, int endingPixel, unsigned char *scanlineBuffer)
{
	for (int p = startPixel; p <= endingPixel; p++)
	{
		unsigned char pixel = scanlineBuffer[p];
		currentPixelPointer[0] = pixel;
		currentPixelPointer++;
	}
}

HDC GDIRenderer::getFrameBufferHDC()
{
	if (this->currentFrame == 0)
		return this->frameBuffer2HDC;  //opposite buffer contains the last completed frame
	else
		return this->frameBuffer1HDC;
}

void GDIRenderer::drawFrame()
{

}