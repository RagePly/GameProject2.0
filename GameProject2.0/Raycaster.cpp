#include "Raycaster.h"


Raycaster::Raycaster(int screenHeight, int screenWidth, int pitch) : 
	screenHeight(screenHeight), 
	screenWidth(screenWidth),
	pitch(pitch) 
{


};
Raycaster::~Raycaster() {};

/*void Raycaster::renderComplete(const Sphere& sphere, unsigned char* pixels) {
	for (int y = 0; y < screenHeight; y++) {
		for (int x = 0; x < screenWidth; x++) {
			if()
		}
	}
}
*/





Camera::Camera(float dstToScr, int screenWidth, int screenHeight, const Float2& dim, const Float3& pos) : 
	posEye(pos.getX(), pos.getY() - dstToScr, pos.getZ()),							
	pix(dim.getX() / (float) screenWidth, dim.getY() / (float) screenHeight),																								
	dim(dim),																							
	pos(pos)
{
	this->dstToScr = dstToScr;
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
	
	indexOffsetX = screenWidth / 2;
	indexOffsetY = screenHeight / 2;

	if (screenWidth % 2 == 0) widthEven = 1;
	else widthEven = 0;

	if (screenHeight % 2 == 0) heightEven = 1;
	else heightEven = 0;
}

Camera::~Camera() {};

int signOf(int i) {
	if (i < 0) return -1;
	else return 1;
}

/*
void Camera::renderComplete(const Sphere& sphere, unsigned char* pixels, int pitch) {
	for (int y = 0; y < screenHeight; y++) {
		for (int x = 0; x < screenWidth; x++) {
			Ray ray(pos, vectorToPixel(x, y), STEP_LENGTH, MAX_LENGTH);
			if (ray.intersects(sphere)) {
				pixels[y * pitch + 4 * x] = 255;
				pixels[y * pitch + 4 * x + 1] = 255;
				pixels[y * pitch + 4 * x + 2] = 255;
			}
			else {
				pixels[y * pitch + 4 * x] = 0;
				pixels[y * pitch + 4 * x + 1] = 0;
				pixels[y * pitch + 4 * x + 2] = 0;
			}
		}
	}
}
*/

void Camera::renderRow(const Sphere* sphere, unsigned char* pixels, int pitch, int row, const Float3& ligth) {
	int y = row;
	for (int x = 0; x < screenWidth; x++) {
		Ray ray(pos, vectorToPixel(x, y), 1, MAX_LENGTH);
		RGB_COLOR color = ray.raytrace(sphere, ligth, 0);

		pixels[y * pitch + 4 * x] = color.BLUE;
		pixels[y * pitch + 4 * x + 1] = color.GREEN;
		pixels[y * pitch + 4 * x + 2] = color.RED;
	}
}

Float3 Camera::vectorToPixel(int x, int y) {
	int relX = x - indexOffsetX;
	int relY = indexOffsetY - y;

	Float3 posToPix(
		relX * pix.getX() - ((float) widthEven * signOf(relX)) * pix.getX() / 2.0f,	//x
		0,																			//y
		relY * pix.getY() - ((float) heightEven * signOf(relY)) * pix.getY() / 2.0f	//z
	);

	return pos + posToPix - posEye;
}

void Camera::print() {

	std::cout << "Screen width: " << screenWidth << std::endl;
	std::cout << "Screen height: " << screenHeight<< std::endl;
	std::cout << "Screen dimensions: "; dim.print();
	std::cout << "Camera position: "; pos.print();
	std::cout << "Eye position: "; posEye.print();
	std::cout << "Distance to screen: " << dstToScr << std::endl;
	std::cout << "Pixel width and height: "; pix.print();
}

Ray::Ray(const Ray& ray) :
	pos(ray.pos),
	dir(ray.dir)
{
	stepLen = ray.stepLen;
	maxDist = ray.maxDist;
	dist = ray.dist;
}

Ray::Ray(const Float3& position, const Float3& direction, float stepLength, float maxDistance) :
	pos(position),
	dir(direction)
{
	stepLen = stepLength;
	maxDist = maxDistance;
	dist = 0.0f;

	dir.setLen(stepLen);
}

Ray::~Ray() {};

char lerpc(int a, int b, float t) {
	int ti = ceil(t * 1000);
	return a + ti * (b - a) / 1000;
}

float lerpf(float a, float b, float t) {
	return a + t * (b - a);
}

RGB_COLOR Ray::raytrace(const Sphere* sphere,const Float3& light, int bounces) {
	while (dist < maxDist) {
		setStepLen(minDist(sphere));
		step();
		if (stepLen <= STEP_LENGTH_THRESHOLD) {

			if (bounces == MAX_BOUNCE_LIMIT) {
				return SPHERE_STD;
			}
			else if (bounces == MAX_BOUNCE_LIMIT - 1 && !BWTEST) {
				Sphere closest = sphere[ioco(sphere)];
				bounces++;
				Float3 norm = closest.norm(pos);
				norm.setLen(closest.radi() + ESCAPE_THRESHOLD);
				norm = closest.position() + norm ;
				setPos(norm);

				changeTarget(light);
				return raytrace(sphere, light, bounces);
			}
			else if (bounces == MAX_BOUNCE_LIMIT - 1) {
				Sphere closest = sphere[ioco(sphere)];
				Float3 norm = closest.norm(pos);
				Float3 toLight = light - pos;
				toLight.norm();
				float dotProduct = toLight.dot(norm);

				if (dotProduct > 0 && dotProduct < LERPTHRESHOLD) {
					char in = lerpc(0, 255, (dotProduct / LERPTHRESHOLD));
					if (closest.col().RED == 1)
						return RGB_COLOR(in, 0, 0);
					else return RGB_COLOR(0, 0, in);
				}
				else if (dotProduct >= LERPTHRESHOLD) {
					if (closest.col().RED == 1) {
						return RGB_COLOR(255, 0, 0);
					}
					else return RGB_COLOR(0, 0, 255);
				}
				else return SKY_COLOR;
				
			}
			else {
				Sphere closest = sphere[ioco(sphere)];
				bounces++;
				Float3 n = closest.norm(pos);
				Float3 pp(n);
				pp.setLen(closest.radi() + ESCAPE_THRESHOLD);
				pp = closest.position() + pp;
				setPos(pp);

				n.mult(2.0f * dir.dot(n));
				Float3 dirp = dir - n;
				setDir(dirp);


				//bouncing color
				Ray bouncingRay(*this);
				RGB_COLOR color1 = bouncingRay.raytrace(sphere, light, bounces);
				changeTarget(light);

				RGB_COLOR color2 = raytrace(sphere, light, MAX_BOUNCE_LIMIT);

				RGB_COLOR color;

				if (color1.BLUE == 50 || color1.BLUE == 200) {
					color.BLUE = color1.BLUE;
					color.RED = color1.RED;
					color.GREEN = color1.GREEN;
				}
				else {
					color.BLUE = color2.BLUE;
					color.RED = color2.RED;
					color.GREEN = color2.GREEN;
				}


				return color;

			}

		}
	}
	if (bounces == MAX_BOUNCE_LIMIT) return SUN_COLOR;
	return SKY_COLOR;
}

float Ray::minDist(const Sphere* spheres) {
	float min = FLT_MAX;
	for (int i = 0; i < 2; i++) {
		float distance = spheres[i].dist(pos);
		if (distance < min) min = distance;
	}
	return min;
}

int Ray::ioco(const Sphere* sphere) {
	float min = FLT_MAX;
	int in = 0;
	for (int i = 0; i < 2; i++) {
		float distance = sphere[i].dist(pos);
		if (distance < min) {
			min = distance;
			in = i;
		}
	}
	return in;
}

void Ray::changeTarget(const Float3& newV) {
	Float3 newDir = newV - pos;
	newDir.setLen(stepLen);
	dir = newDir;
}

void Ray::resetDist() {
	dist = 0.0f;
}

void Ray::setMaxDist(float newMaxDistance) {
	maxDist = newMaxDistance;
}

void Ray::setStepLen(float newStepLen) {
	if (newStepLen >= MIN_STEP_LENGTH) {
		stepLen = newStepLen;
	}
	else stepLen = MIN_STEP_LENGTH;
	dir.setLen(stepLen);
}


void Ray::step() {
	pos.add(dir);
	dist += stepLen;
}

void Ray::print() {
	std::cout << "Step-length: " << stepLen << std::endl;
	std::cout << "Max distance: " << maxDist << std::endl;
	std::cout << "Distance traveled: " << dist << std::endl;
	std::cout << "Direction vector: "; dir.print();
	std::cout << "Position: "; pos.print();
	std::cout << "Distance according to length from origo: " << pos.len() << std::endl;

}

void Ray::setPos(const Float3& pos) {
	this->pos = pos;
}

void Ray::setDir(const Float3& dir) {
	this->dir = dir;
}

