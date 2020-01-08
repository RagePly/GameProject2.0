#pragma once
#include "Raycaster.h"

class RastCam {
public:
	Int2 pixDim;
	Float2 screenDim;
	float distToScreen;

	RastCam();
	RastCam(Gobject* cameraObj, Int2 pixDim, Float2 screenDim, float distToScreen);
	~RastCam();

	float getX() const;
	float getY() const;
	float getZ() const;

	float getRotX() const;
	float getRotZ() const;

	int getpW() const;
	int getpH() const;
	float getsW() const;
	float getsH() const;

private:
	Gobject* camerObj;
};

class Rasterizer {
public:
	Rasterizer();
	~Rasterizer();

	void addCamera(RastCam* cam);
	void addGameWorldReference(World* gWorld);

	void renderImage(unsigned char* pixels);
private:
	World* gWorld;
	RastCam* rastCam;
};