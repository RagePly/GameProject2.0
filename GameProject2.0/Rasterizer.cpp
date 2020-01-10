#include "Rasterizer.h"


RastCam::RastCam()
	:
	pixDim(0, 0),
	screenDim({ 0,0 }),
	distToScreen(0.0f),
	updated(true)
{
	camerObj = nullptr;
}

RastCam::RastCam(Gobject* camerObj, Int2 pixDim, Float2 screenDim, float distToScreen)
	:
	pixDim(pixDim),
	screenDim(screenDim),
	distToScreen(distToScreen),
	updated(true)
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
	CamStat cam = rastCam->getCamStat();

	for (int i = 0; i < gWorld->nrGobj; i++) {
		Gobject obj = gWorld->getAllGobj()[i];
		if (obj.id == id_camerobj) continue;

		PointStat pointStat = tfPToScr(obj.tf.pos);

		if (pointStat.behindScreen || !pointStat.insideScreen) continue;
		
		//Draw if all conditions are met
		pixels[pointStat.pos.y * camStat.pixW * 4 + pointStat.pos.x * 4] = 255;
		pixels[pointStat.pos.y * camStat.pixW * 4 + pointStat.pos.x * 4 + 1] = 255;
		pixels[pointStat.pos.y * camStat.pixW * 4 + pointStat.pos.x * 4 + 2] = 255;
	}

	
}

void Rasterizer::updateCamStats() {
	if (rastCam->isUpdated()) {
		camStat = rastCam->getCamStat();
		rastCam->updateRecieved();
	}
}

PointStat Rasterizer::tfPToScr(const Float3& point) const {

	PointStat stat;
	//moving origin to camera pos
	float xp = point.getX() - camStat.cx;
	float yp = point.getY() - camStat.cy;
	float zp = point.getZ() - camStat.cz;

	//rotating around origo and move origo to the eye (which is distToScr neg y direction from camera)
	xp = camStat.cos1 * camStat.cos2 * xp - camStat.cos1 * camStat.sin2 * yp;
	yp = camStat.cos1 * camStat.sin2 * xp + camStat.cos1 * camStat.cos2 * yp - camStat.sin1 * yp + camStat.distToScr;
	zp = camStat.sin1 * xp + camStat.sin1 * yp + camStat.cos1 * zp;

	float s = camStat.distToScr / yp; //find the scaling required to land on the screen plane

	if (s < 0.0f || s > 1.0f) stat.behindScreen = true; //if the point is behind the screen

	//scale vector onto screen plane and invert y (previous z) axis, also shifting origo to top left
	xp = s * xp + camStat.scrW / 2.0f;
	yp = -s * zp + camStat.scrH / 2.0f; //pretty sure on the + here, check notebook

	//scale to fit screen
	xp *= camStat.pixW / camStat.scrW;
	yp *= camStat.pixH / camStat.scrH;

	int x = (int)floor(xp);
	int y = (int)floor(yp);

	if (x > camStat.pixW || x < 0 || y > camStat.pixH || y < 0) stat.insideScreen = false; //final criteria

	stat.pos = Int2(x, y);

	return stat;
}
