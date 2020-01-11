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
	float dist(const Float3& destination) const;
	float getX() const;
	float getY() const;
	float getZ() const;
	void norm();
	void setLen(float length);
	void print() const;

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
	void print() const;
private:
	float x;
	float y;
};


class Int2 {
public:
	int x, y;

	Int2();
	Int2(const Int2& v);
	Int2(int x, int y);
	Int2(const Float2& v);
	~Int2();

	void add(const Int2& v);
};


/*
float cos(float theta);
float sin(float theta);
*/