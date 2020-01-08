#include "Rasterizer.h"


RastCam::RastCam()
	:
	pixDim(0,0),
	screenDim({ 0,0 })
{
	camerObj = nullptr;
}

RastCam::RastCam(Gobject* camerObj, Int2 pixDim, Float2 screenDim)
	:
	pixDim(pixDim),
	screenDim(screenDim)
{
	this->camerObj = camerObj;
}

RastCam::~RastCam() {
	delete camerObj;
}
