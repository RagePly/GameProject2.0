#include "Geometry.h"



//Float3

Float3::Float3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Float3::Float3(const Float3& f3) {
	x = f3.x;
	y = f3.y;
	z = f3.z;
}

Float3::~Float3() {};

void Float3::add(const Float3& v) {
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
}

void Float3::sub(const Float3& v) {
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
}

void Float3::div(float f) {
	if (f != 0.0f){
	x = x / f;
	y = y / f;
	z = z / f;
	}
	else {
		if (x < 0.0f) x = -FLT_MAX;
		else if (x != 0.0f) x = FLT_MAX;
		if (y < 0.0f) y = -FLT_MAX;
		else if (y != 0.0f) y = FLT_MAX;
		if (z < 0.0f) z = -FLT_MAX;
		else if (z != 0.0f) z = FLT_MAX;
	}
}

void Float3::mult(float f) {
	x *= f;
	y *= f;
	z *= f;
}

float Float3::dot(const Float3& v) {
	return x * v.x + y * v.y + z * v.z;
}

Float3* Float3::clone() const{
	return new Float3(x, y, z);
}

float Float3::lensq() const{
	return x * x + y * y + z * z;
}

float Float3::len() const {
	return sqrtf(lensq());
}

float Float3::distsq(const Float3& v) const {
	float _x = x - v.x;
	float _y = y - v.y;
	float _z = z - v.z;
	return _x * _x + _y * _y + _z * _z;
}

float Float3::getX() const{
	return x;
}
float Float3::getY() const{
	return y;
}
float Float3::getZ() const {
	return z;
}

void Float3::norm(){
	float div = sqrtf(lensq());
	if (div != 0.0f) {
		x = x / div;
		y = y / div;
		z = z / div;
	}
}

void Float3::setLen(const float l) {
	norm();
	mult(l);
}

void Float3::print() {
	std::cout << x << ", " << y << ", " << z << std::endl;
}

Float3 Float3::operator+(const Float3& f) const {
	return Float3(x + f.x, y + f.y, z + f.z);
}

Float3 Float3::operator-(const Float3& f) const {
	return Float3(x - f.x, y - f.y, z - f.z);
}



//float 2

Float2::Float2(float x, float y) {
	this->x = x;
	this->y = y;
}

Float2::Float2(const Float2& f3) {
	x = f3.x;
	y = f3.y;
}

Float2::~Float2() {

}

void Float2::add(Float2& v) {
	x = x + v.x;
	y = y + v.y;
}

void Float2::sub(Float2& v) {
	x = x - v.x;
	y = y - v.y;
}

void Float2::div(float f) {
	if (f != 0.0f) {
		x = x / f;
		y = y / f;
	}
	else {
		if (x < 0.0f) x = -FLT_MAX;
		else if (x != 0.0f) x = FLT_MAX;
		if (y < 0.0f) y = -FLT_MAX;
		else if (y != 0.0f) y = FLT_MAX;
	}
}

void Float2::mult(float f) {
	x *= f;
	y *= f;
}

Float2* Float2::clone() const{
	return new Float2(x, y);
}

float Float2::lensq() const {
	return x * x + y * y;
}

float Float2::distsq(Float2& v) const {
	float _x = x - v.x;
	float _y = y - v.y;
	return _x * _x + _y * _y;
}

float Float2::getX() const {
	return x;
}

float Float2::getY() const {
	return y;
}

void Float2::print(){
	std::cout << x << ", " << y << std::endl;
}


//sphere

Sphere::Sphere() :
	pos(0.0f,0.0f,0.0f),
	color(0.0f,0.0f,0.0f)
{
	radius = 0.0f;
}

Sphere::Sphere(const Float3& position, float radius, const RGB_COLOR& color) : 
	pos(position),
	color(color)
{
	this->radius = radius;
}

Sphere::~Sphere() {};

Float3 Sphere::norm(const Float3& v) const {
	Float3 w = v - pos;
	w.norm();
	return w;
}

RGB_COLOR Sphere::col() const {
	return RGB_COLOR(color);
}

bool Sphere::intersects(const Float3& v) const {
	return pos.distsq(v) <= radius * radius;
}

void Sphere::print() {
	std::cout << "Position: "; pos.print();
	std::cout << "Radius: " << radius << std::endl;
}

const Float3& Sphere::position() const {
	return pos;
}

float Sphere::radi() const {
	return radius;
}

float Sphere::dist(const Float3& float3) const {
	return sqrtf(pos.distsq(float3)) - radius;
}
