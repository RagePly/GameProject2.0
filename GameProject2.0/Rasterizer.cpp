#include "Rasterizer.h"


RastCam::RastCam()
	:
	pixDim(0, 0),
	screenDim({ 0,0 }),
	distToScreen(0.0f)
{
	camerObj = nullptr;
}

RastCam::RastCam(Gobject* camerObj, Int2 pixDim, Float2 screenDim, float distToScreen)
	:
	pixDim(pixDim),
	screenDim(screenDim),
	distToScreen(distToScreen)

{
	this->camerObj = camerObj;
}

RastCam::~RastCam() {
	delete camerObj;
}

float RastCam::getX() const{
	return camerObj->tf.pos.getX();
};
float RastCam::getY() const{
	return camerObj->tf.pos.getY();
};
float RastCam::getZ() const{
	return camerObj->tf.pos.getZ();
};

float RastCam::getRotX() const {
	return camerObj->tf.rot.getX();
}

float RastCam::getRotZ() const {
	return camerObj->tf.rot.getY(); //Think about changing the rotation to a float3 sometime
}

int RastCam::getpW() const{
	return pixDim.x;
};
int RastCam::getpH() const{
	return pixDim.y;
};
float RastCam::getsW() const{
	return screenDim.getX();
};
float RastCam::getsH() const{
	return screenDim.getY();
};


Rasterizer::Rasterizer() {}
Rasterizer::~Rasterizer() {
	delete gWorld;
	delete rastCam;
}

void Rasterizer::addCamera(RastCam* cam) {
	rastCam = cam;
}

void Rasterizer::addGameWorldReference(World* gWorld) {
	this->gWorld = gWorld;
}

void Rasterizer::renderImage(unsigned char* pixels) {
	float cx = rastCam->getX();
	float cy = rastCam->getY();
	float cz = rastCam->getZ();
	float distToScr = rastCam->distToScreen;
	float scrH = rastCam->getsH();
	float scrW = rastCam->getsW();
	int pixH = rastCam->getpH();
	int pixW = rastCam->getpW();

	float cos1 = cos(-rastCam->getRotX());
	float cos2 = cos(-rastCam->getRotZ());
	float sin1 = sin(-rastCam->getRotX());
	float sin2 = sin(-rastCam->getRotZ());

	for (int i = 0; i < gWorld->nrGobj; i++) {
		Gobject obj = gWorld->getAllGobj()[i];
		if (obj.id == id_camerobj) continue;

		//moving origin to camera pos
		float xp = obj.tf.pos.getX() - cx;
		float yp = obj.tf.pos.getY() - cy;
		float zp = obj.tf.pos.getZ() - cz;

		//rotating around origo and move origo to the eye (which is distToScr neg y direction from camera)
		xp = cos1 * cos2 * xp - cos1 * sin2 * yp;
		yp = cos1 * sin2 * xp + cos1 * cos2 * yp - sin1 * yp + distToScr;
		zp = sin1 * xp + sin1 * yp + cos1 * zp;

		float s = distToScr / yp; //find the scaling required to land on the screen plane

		if (s < 0.0f || s > 1.0f) continue; //if the point is behind the screen

		//scale vector onto screen plane and invert y (previous z) axis, also shifting origo to top left
		xp = s*xp + scrW / 2.0f;
		yp = -s * zp + scrH / 2.0f; //pretty sure on the + here, check notebook

		//scale to fit screen
		xp *= pixW / scrW;
		yp *= pixH / scrH;

		int x = (int)floor(xp);
		int y = (int)floor(yp);

		if (x > pixW || x < 0 || y > pixH || y < 0) continue; //final criteria

		//DRAW!
		pixels[y * pixW * 4 + x * 4] = 255;
		pixels[y * pixW * 4 + x * 4 + 1] = 255;
		pixels[y * pixW * 4 + x * 4 + 2] = 255;
	}

}