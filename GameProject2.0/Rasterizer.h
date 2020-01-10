#pragma once
#include "Raycaster.h"


struct CamStat {
public:
	float cx;
	float cy;
	float cz;
	float distToScr;
	float scrH;
	float scrW;
	int pixH;
	int pixW;
	float cos1;
	float cos2;
	float sin1;
	float sin2;
};



struct PointStat {
	Int2 pos;
	bool insideScreen = true;
	bool behindScreen = false;
};


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

	CamStat getCamStat() const;
	bool isUpdated() const { return updated; };
	void updateRecieved() { updated = false; };

private:
	bool updated;
	Gobject* camerObj;
};


class Rasterizer {
public:
	Rasterizer();
	~Rasterizer();

	void addCamera(RastCam* cam);
	void addGameWorldReference(World* gWorld);
	void addPainter(Painter* painter);
	void updateCamStats();
	void renderImage();
	PointStat tfPToScr(const Float3 &point) const; //Tranfsform point to screen
private:
	World* gWorld;
	RastCam* rastCam;
	CamStat camStat;
	Painter* painter;
};