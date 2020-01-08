#pragma once
#include "Raycaster.h"

class RastCam {
public:
	Int2 pixDim;
	Float2 screenDim;

	RastCam();
	RastCam(Gobject* cameraObj, Int2 pixDim, Float2 screenDim);
	~RastCam();

private:
	Gobject* camerObj;
};