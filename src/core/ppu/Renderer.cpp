#include "Renderer.h"


Renderer::Renderer()
{
	this->redrawListener = 0;
}


Renderer::~Renderer()
{
}


void Renderer::setRedrawListener(RedrawListener* listener)
{
	this->redrawListener = listener;
}

void Renderer::requestRedraw()
{
	this->redrawListener->onRedrawRequest();
}


RedrawListener::RedrawListener()
{

}

RedrawListener::~RedrawListener()
{

}