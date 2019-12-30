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


