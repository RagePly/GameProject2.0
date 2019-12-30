#pragma once
#include "ObjectWorld.h"



class Raycaster {
public:
	Raycaster(int screenWidth, int screenHeigh, int pitch);
	~Raycaster();

	//void renderComplete(const Sphere& sphere, unsigned char* pixels);

private:
	const int screenWidth, screenHeight;
	const int pitch;
};


class Ray {
public:
	Ray(const Ray& ray);
	Ray(const Float3& position, const Float3& direction, float stepLength, float maxDistance);
	~Ray();



	RGB_COLOR raytrace(const Sphere* sphere, const Float3& ligth, int bounces);
	void step();
	void changeTarget(const Float3& newTarget);
	void resetDist();
	void setMaxDist(float newMaxDistance);
	void setPos(const Float3& position);
	void setDir(const Float3& direction);
	void setStepLen(float newLen);
	void print();
	float minDist(const Sphere* sphere);
	int ioco(const Sphere* sphere); //index of closest object

private:
	Float3 pos;
	Float3 dir;
	float stepLen;
	const float minStepLen = MIN_STEP_LENGTH;
	float dist;
	float maxDist;
};


class Camera {
public:
	Camera(float distanceToScreen, int screenWidth, int screenHeight, const Float2& dimensions, const Float3& position);
	~Camera();
	void renderComplete(const Sphere& sphere, unsigned char* pixels, int pitch);
	void renderRow(const Sphere* sphere, unsigned char* pixels, int pitch, int row, const Float3& lightSource);
	bool pixelIntersects(int x, int y, const Sphere& sphere);
	Float3 vectorToPixel(int x, int y);

	void print();

private:
	int indexOffsetX, indexOffsetY; //TODO: make it a int2 class
	int screenWidth;
	int screenHeight;
	int widthEven, heightEven;
	Float2 pix;
	Float2 dim;
	Float3 pos;
	Float3 posEye;
	float dstToScr; //TODO: it should be an angle, FOV
};