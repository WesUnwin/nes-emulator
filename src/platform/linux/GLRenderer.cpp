#include "GLRenderer.h"

#include <iostream>

GLRenderer::GLRenderer()
{
	this->currentFrame = 0;
	this->currentPixelPointer = &(this->frameBuffer1[0]);

	this->setMasterPalette((PPUCore::MASTERPALENTRY*) PPUCore::DEFAULT_MASTER_PALETTE);
}

GLRenderer::~GLRenderer()
{

}

void GLRenderer::newFrame()
{
	this->currentFrame = this->currentFrame == 0 ? 1 : 0;  //Toggle between buffers
	if (this->currentFrame == 0)
		currentPixelPointer = &(this->frameBuffer1[0]);
	else
		currentPixelPointer = &(this->frameBuffer2[0]);
}

void GLRenderer::drawPixels(int scanline, int startPixel, int endingPixel, unsigned char *scanlineBuffer)
{
	for (int p = startPixel; p <= endingPixel; p++)
	{
		unsigned char masterPalOffset = scanlineBuffer[p];
		currentPixelPointer[0] = this->masterPal[masterPalOffset];
		currentPixelPointer ++;
	}
}

void GLRenderer::drawFrame()
{
	
}

void* GLRenderer::getCompletedFramePixels()
{
	if( this->currentFrame == 0)
		return frameBuffer2;
	else
		return frameBuffer1;
}

void GLRenderer::setMasterPalette(PPUCore::MASTERPALENTRY* colors)
{
	for(int i = 0; i<64; i++)
		this->masterPal[i] = ((((unsigned int) colors[i].Blue) << 16) | (((unsigned int) colors[i].Green) << 8) | ((unsigned int) colors[i].Red));
}
