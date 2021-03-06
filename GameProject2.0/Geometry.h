#pragma once
#include "Color.h"


class Float3 {
public:
	Float3(float x, float y, float z);
	Float3(const Float3&);
	~Float3();

	void add(const Float3& float3);
	void sub(const Float3& float3);
	void div(float);
	void mult(float);
	float dot(const Float3& float3);
	Float3* clone() const;
	float lensq() const;
	float len() const;
	float distsq(const Float3& destination) const;
	float getX() const;
	float getY() const;
	float getZ() const;
	void norm();
	void setLen(float length);
	void print();

	Float3 operator + (const Float3&) const;
	Float3 operator - (const Float3&) const;

private:
	float x;
	float y;
	float z;
};

class Float2 {
public:
	Float2(float x, float y);
	Float2(const Float2&);
	~Float2();

	void add(Float2&);
	void sub(Float2&);
	void div(float);
	void mult(float);
	Float2* clone() const;
	float lensq() const;
	float distsq(Float2&) const;
	float getX() const;
	float getY() const;
	void print();
private:
	float x;
	float y;
};

class Sphere {
public:
	Sphere();
	Sphere(const Float3& position, float radius, const RGB_COLOR& color);
	~Sphere();

	Float3 norm(const Float3& float3) const;
	RGB_COLOR col() const;
	bool intersects(const Float3&) const ;
	void print();
	float radi() const;
	float dist(const Float3& float3) const;
	const Float3& position() const;

private:
	Float3 pos;
	float radius;
	RGB_COLOR color;
};