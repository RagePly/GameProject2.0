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
	update = true;
}

void Float3::sub(const Float3& v) {
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
	update = true;
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
	update = true;
}

void Float3::mult(float f) {
	x *= f;
	y *= f;
	z *= f;
	update = true;
}

float Float3::dot(const Float3& v) {
	return x * v.x + y * v.y + z * v.z;
}

void Float3::set(const Float3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	update = true;
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

float Float3::dist(const Float3& v) const {
	return sqrtf(distsq(v));
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
	update = true;
}

void Float3::setLen(const float l) {
	norm();
	mult(l);
	update = true;
}

void Float3::print() const {
	std::cout << x << ", " << y << ", " << z << std::endl;
}

Float3 Float3::operator+(const Float3& f) const {
	return Float3(x + f.x, y + f.y, z + f.z);
}

Float3 Float3::operator-(const Float3& f) const {
	return Float3(x - f.x, y - f.y, z - f.z);
}

bool Float3::isUpdated() { //TODO:if there is a need for it change this in the future;
	bool up = update;
	update = false;
	return up;
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

void Float2::add(const Float2& v) {
	x = x + v.x;
	y = y + v.y;
	update = true;
}

void Float2::sub(const Float2& v) {
	x = x - v.x;
	y = y - v.y;
	update = true;
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
	update = true;
}

void Float2::mult(float f) {
	x *= f;
	y *= f;
	update = true;
}

void Float2::set(const Float2& v) {
	x = v.x;
	y = v.y;
	update = true;
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

void Float2::print() const{
	std::cout << x << ", " << y << std::endl;
}

bool Float2::isUpdated() { //TODO:if there is a need for it change this in the future;
	bool up = update;
	update = false;
	return up;
}

Int2::Int2() 
	:
	x(0),
	y(0)
{}

Int2::Int2(const Int2& v)
	:
	x(v.x),
	y(v.y)
{}

Int2::Int2(int x, int y)
	:
	x(x),
	y(y)	
{}

Int2::Int2(const Float2& v)
	:
	x((int) v.getX()),
	y((int) v.getY())
{}

Int2::~Int2()
{}


void Int2::add(const Int2& v) {
	x += v.x;
	y += v.y;
}





void fmat3x3::prnt() {
	std::cout << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << std::endl;
	std::cout << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << std::endl;
	std::cout << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << std::endl;
}

void fmat3x1::prnt() {

	std::cout << mat[0] << ", " << mat[1] << ", " << mat[2] << std::endl;
}

void fmat3x3mult(const fmat3x3& first, const fmat3x3& second, fmat3x3& mod) {
	mod.mat[0][0] = first.mat[0][0] * second.mat[0][0] + first.mat[0][1] * second.mat[1][0] + first.mat[0][2] * second.mat[2][0];
	mod.mat[0][1] = first.mat[0][0] * second.mat[0][1] + first.mat[0][1] * second.mat[1][1] + first.mat[0][2] * second.mat[2][1];
	mod.mat[0][2] = first.mat[0][0] * second.mat[0][2] + first.mat[0][1] * second.mat[1][2] + first.mat[0][2] * second.mat[2][2];
	mod.mat[1][0] = first.mat[1][0] * second.mat[0][0] + first.mat[1][1] * second.mat[1][0] + first.mat[1][2] * second.mat[2][0];
	mod.mat[1][1] = first.mat[1][0] * second.mat[0][1] + first.mat[1][1] * second.mat[1][1] + first.mat[1][2] * second.mat[2][1];
	mod.mat[1][2] = first.mat[1][0] * second.mat[0][2] + first.mat[1][1] * second.mat[1][2] + first.mat[1][2] * second.mat[2][2];
	mod.mat[2][0] = first.mat[2][0] * second.mat[0][0] + first.mat[2][1] * second.mat[1][0] + first.mat[2][2] * second.mat[2][0];
	mod.mat[2][1] = first.mat[2][0] * second.mat[0][1] + first.mat[2][1] * second.mat[1][1] + first.mat[2][2] * second.mat[2][1];
	mod.mat[2][2] = first.mat[2][0] * second.mat[0][2] + first.mat[2][1] * second.mat[1][2] + first.mat[2][2] * second.mat[2][2];
}



void fmat3x3mul3x1(const fmat3x3& tf, const fmat3x1& tg, fmat3x1& mod) {
	mod.mat[0] = tf.mat[0][0] * tg.mat[0] + tf.mat[0][1] * tg.mat[1] + tf.mat[0][2] * tg.mat[2];
	mod.mat[1] = tf.mat[1][0] * tg.mat[0] + tf.mat[1][1] * tg.mat[1] + tf.mat[1][2] * tg.mat[2];
	mod.mat[2] = tf.mat[2][0] * tg.mat[0] + tf.mat[2][1] * tg.mat[1] + tf.mat[2][2] * tg.mat[2];
}