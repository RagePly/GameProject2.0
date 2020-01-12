#include "Color.h"

RGB_COLOR::RGB_COLOR() {
	RED = 0;
	GREEN = 0;
	BLUE = 0;
}

RGB_COLOR::RGB_COLOR(unsigned char RED, unsigned char GREEN, unsigned char BLUE) {
	this->RED = RED;
	this->GREEN = GREEN;
	this->BLUE = BLUE;
}

RGB_COLOR::RGB_COLOR(const RGB_COLOR& color) {
	RED = color.RED;
	GREEN = color.GREEN;
	BLUE = color.BLUE;
}

RGB_COLOR::~RGB_COLOR(){}


Painter::Painter() :
	lockedPixels(nullptr),
	working(false),
	w(0),h(0)
{}

Painter::Painter(int w, int h) :
	lockedPixels(nullptr),
	working(false),
	w(w), h(h)
{}

Painter::~Painter() {}

void Painter::beginDrawing(unsigned char* lockedPixels) {
	this->lockedPixels = lockedPixels;
	working = true;
}

void Painter::clearCanvas() {
	if (working) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				fastDraw(x, y);
			}
		}
	}
	
}

void Painter::draw(int x, int y) {

	if (!(x < 0 || y < 0 || x > w || y > h) && working) {
		lockedPixels[y * 4 * w + x * 4] = 255;
		lockedPixels[y * 4 * w + x * 4 + 1] = 255;
		lockedPixels[y * 4 * w + x * 4 + 2] = 255;
	}
}

void Painter::fastDraw(int x, int y) {
	lockedPixels[y * 4 * w + x * 4] = 0;
	lockedPixels[y * 4 * w + x * 4 + 1] = 0;
	lockedPixels[y * 4 * w + x * 4 + 2] = 0;
}
