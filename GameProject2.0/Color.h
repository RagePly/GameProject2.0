#pragma once
#include "Settings.h"


struct RGB_COLOR {
public:
	RGB_COLOR();
	RGB_COLOR(unsigned char RED, unsigned char GREEN, unsigned char BLUE);
	RGB_COLOR(const RGB_COLOR& color);
	~RGB_COLOR();

	unsigned char RED;
	unsigned char GREEN;
	unsigned char BLUE;
};

const static RGB_COLOR SKY_COLOR(0, 0, 200);
const static RGB_COLOR SUN_COLOR(255, 255, 255);
const static RGB_COLOR SPHERE_STD(0, 0, 50);
