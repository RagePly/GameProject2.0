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


CamStat RastCam::getCamStat() const {
	CamStat newStats;
	newStats.cx = getX();
	newStats.cy = getY();
	newStats.cz = getZ();
	newStats.distToScr = distToScreen;
	newStats.scrH = getsH();
	newStats.scrW = getsW();
	newStats.pixH = getpH();
	newStats.pixW = getpW();

	newStats.cos1 = cos(-getRotX());
	newStats.cos2 = cos(-getRotZ());
	newStats.sin1 = sin(-getRotX());
	newStats.sin2 = sin(-getRotZ());


	return newStats;
}


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
	CamStat stat = rastCam->getCamStat();

	for (int i = 0; i < gWorld->nrGobj; i++) {
		Gobject obj = gWorld->getAllGobj()[i];
		if (obj.id == id_camerobj) continue;

		//moving origin to camera pos
		float xp = obj.tf.pos.getX() - stat.cx;
		float yp = obj.tf.pos.getY() - stat.cy;
		float zp = obj.tf.pos.getZ() - stat.cz;

		//rotating around origo and move origo to the eye (which is distToScr neg y direction from camera)
		xp = stat.cos1 * stat.cos2 * xp - stat.cos1 * stat.sin2 * yp;
		yp = stat.cos1 * stat.sin2 * xp + stat.cos1 * stat.cos2 * yp - stat.sin1 * yp + stat.distToScr;
		zp = stat.sin1 * xp + stat.sin1 * yp + stat.cos1 * zp;

		float s = stat.distToScr / yp; //find the scaling required to land on the screen plane

		if (s < 0.0f || s > 1.0f) continue; //if the point is behind the screen

		//scale vector onto screen plane and invert y (previous z) axis, also shifting origo to top left
		xp = s*xp + stat.scrW / 2.0f;
		yp = -s * zp + stat.scrH / 2.0f; //pretty sure on the + here, check notebook

		//scale to fit screen
		xp *= stat.pixW / stat.scrW;
		yp *= stat.pixH / stat.scrH;

		int x = (int)floor(xp);
		int y = (int)floor(yp);

		if (x > stat.pixW || x < 0 || y > stat.pixH || y < 0) continue; //final criteria

		//DRAW!
		pixels[y * stat.pixW * 4 + x * 4] = 255;
		pixels[y * stat.pixW * 4 + x * 4 + 1] = 255;
		pixels[y * stat.pixW * 4 + x * 4 + 2] = 255;
	}

	

}

