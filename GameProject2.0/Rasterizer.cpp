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

bool RastCam::isUpdated() { //THIS IS SO DUMB AT THE MOMENT
	updated = camerObj->tf.pos.isUpdated() || camerObj->tf.rot.isUpdated();
	return updated;
}


Rasterizer::Rasterizer():
	gWorld(nullptr),
	rastCam(nullptr),
	painter(nullptr),
	camStat()
{}
Rasterizer::~Rasterizer() {
	delete rastCam;
}

void Rasterizer::addCamera(RastCam* cam) {
	rastCam = cam;
}

void Rasterizer::addGameWorldReference(World* gWorld) {
	this->gWorld = gWorld;
}

void Rasterizer::addPainter(Painter* painter) {
	this->painter = painter;
}

void Rasterizer::renderImage() {
	CamStat cam = rastCam->getCamStat();

	for (int i = 0; i < gWorld->nrGobj; i++) {
		Gobject obj = gWorld->getAllGobj()[i];
		if (obj.id == id_camerobj) continue;

		PointStat pointStat = tfPToScr(obj.tf.pos);

		if (pointStat.behindScreen || !pointStat.insideScreen) continue; //find actual use for this
		
		//Draw if all conditions are met
		painter->draw(pointStat.pos.x, pointStat.pos.y);
	}
}

void Rasterizer::updateCamStats() {
	if (rastCam->isUpdated()) {
		camStat = rastCam->getCamStat();
		rastCam->updateRecieved();
	}
}

#define V1X camStat.cos1 * camStat.cos2 * xp - camStat.cos1 * camStat.sin2 * yp
#define V1Y camStat.cos1 * camStat.sin2 * xp + camStat.cos1 * camStat.cos2 * yp - camStat.sin1 * yp
#define V1Z camStat.sin1 * xp + camStat.sin1 * yp + camStat.cos1 * zp

#define V2X xp * camStat.cos2 - yp * camStat.sin2
#define V2Y xp * camStat.cos1 * camStat.sin2 + yp * camStat.cos1 * camStat.cos2 - zp * camStat.sin1
#define V2Z xp * camStat.sin1 * camStat.sin2 + yp * camStat.sin1 * camStat.cos2 + zp * camStat.cos1

PointStat Rasterizer::tfPToScr(const Float3& point) const {

	PointStat stat;
	//moving origin to camera pos
	float xp = point.getX() - camStat.cx;
	float yp = point.getY() - camStat.cy;
	float zp = point.getZ() - camStat.cz;

	fmat3x1 p;
	p.mat[0] = xp;
	p.mat[1] = yp;
	p.mat[2] = zp;

	//rotating around origo and move origo to the eye (which is distToScr neg y direction from camera)

	//rotate x
	/*
		|	1	0	0		|
	Rx	|	0	cos	-sin	|
		|	0	sin	cos		|
	*/

	fmat3x3 Rx;
	Rx.mat[0][0] = 1.0f;
	Rx.mat[1][1] = camStat.cos1;
	Rx.mat[1][2] = - camStat.sin1;
	Rx.mat[2][1] = camStat.sin1;
	Rx.mat[2][2] = camStat.cos1;

	/*
		|	cos	-sin	0	|
	Rz	|	sin	cos		0	|
		|	0	0		1	|
	*/

	fmat3x3 Rz;
	Rz.mat[0][0] = camStat.cos2;
	Rz.mat[0][1] = -camStat.sin2;
	Rz.mat[1][0] = camStat.sin2;
	Rz.mat[1][1] = camStat.cos2;
	Rz.mat[2][2] = 1.0f;

	fmat3x3 Rzx;
	fmat3x1 pP;
	fmat3x3mult(Rz, Rx, Rzx);
	fmat3x3mul3x1(Rzx, p, pP);

	pP.mat[1] += camStat.distToScr;
	/*
	xp = V1X;
	yp = V1Y + camStat.distToScr;
	zp = V1Z;
	*/

	xp = pP.mat[0];
	yp = pP.mat[1];
	zp = pP.mat[2];

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

void Rasterizer::renderBackground() {
	painter->clearCanvas();
}

void Rasterizer::drawLine(const Float2& a, const Float2& b) {
	Int2 p0(a);
	Int2 p1(b);

	drawLine(p0, p1);
}

void Rasterizer::drawLine(const Int2& a, const Int2& b) {
	Int2 p0(a);
	Int2 p1(b);

	if (p0.x > p1.x) { //correcting order
		p1 = p0;
		p0 = Int2(b);
	}

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;


	//finding the sign of the gradient
	bool positive = true;
	if (dy < 0) positive = false;

	//dominant direction
	bool xdom = true;
	if ((positive && dy > dx) || (!positive && -dy > dx)) xdom = false;

	//Deciding the movement
	Int2 add1, add2;
	int sign;
	if (positive) sign = 1;
	else sign = -1;

	if (xdom) {
		add1.x = 1;
		add2.x = 1;

		add1.y = 0;
		add2.y = sign;
	}
	else {
		add1.x = 0;
		add2.x = 1;

		add1.y = sign;
		add2.y = sign;
	}

	//Simple logic for the bounds of the for loop
	int bound1, bound2;

	if (xdom) {
		bound1 = p0.x;
		bound2 = p1.x;
	}
	else if(positive){
		bound1 = p0.y;
		bound2 = p1.y;
	}
	else {
		bound1 = -p0.y;
		bound2 = -p1.y;
	}


	int C = dx * p0.y - dy * p0.x;
	
	
	//The little traveler
	Int2 pos(p0);

	for (int i = bound1; i < bound2 + 1; i++) {
		int op1 = abs(dy * (pos.x + add1.x) - dx * (pos.y + add1.y) + C);
		int op2 = abs(dy * (pos.x + add2.x) - dx * (pos.y + add2.y) + C);

		if (op2 < op1) {
			pos.add(add2);
			painter->draw(pos.x, pos.y);
		}
		else {
			pos.add(add1);
			painter->draw(pos.x, pos.y);
		}

	}
}