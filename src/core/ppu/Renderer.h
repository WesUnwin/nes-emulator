#pragma once


class RedrawListener
{
public:
	RedrawListener();
	~RedrawListener();

	virtual void onRedrawRequest() = 0;
};



/**
* An abstract class representing the requirements of a graphical renderer.
* A graphical renderer is reponsible for drawing from the scanline buffer to
* some concrete form of bitmap/video memory that will be used to display a frame.
* A renderer should also implement a master palette of the complete set of 64 possible colours
* the PPU can output.
*/
class Renderer
{
public:
	Renderer();
	~Renderer();

	//Invoked on the renderer whenever a new PPU Frame has begun. The renderer
	//should complete actions such as resetting its pixel pointer to the begginning of the internal bitmap
	//and internally swapping buffers if implementing buffering schemes like double buffering.
	virtual void newFrame() = 0;

	// *pixels is a pointer to the first byte, in an array of bytes, each representing a pixel, where the lower 6 bits of each
	//  byte is an index into the master palette representing a colour to draw to the screen.
	virtual void drawPixels(int scanline, int startPixel, int endingPixel, unsigned char *scanlineBuffer ) = 0;

	//Invoked on the renderer, whenever the renderer needs refresh the screen (draw the contents of the internal buffer/bitmap to the screen)
	//Called when needed based on screen refresh frame.
	//If implementing double buffering, draw the last complete frame whenver this is called to the screen.
	virtual void drawFrame() = 0;

	void setRedrawListener(RedrawListener* listener);

	void requestRedraw();
protected:
	RedrawListener* redrawListener;

};


