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
	void set(const Float3& target);
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
	
	bool isUpdated();

	Float3 operator + (const Float3&) const;
	Float3 operator - (const Float3&) const;

private:
	float x;
	float y;
	float z;

	bool update;
};

class Float2 {
public:
	Float2(float x, float y);
	Float2(const Float2&);
	~Float2();

	void add(const Float2&);
	void sub(const Float2&);
	void div(float);
	void mult(float);
	void set(const Float2& target);
	Float2* clone() const;
	float lensq() const;
	float distsq(Float2&) const;
	float getX() const;
	float getY() const;
	void print() const;

	bool isUpdated();
private:
	float x;
	float y;

	bool update;
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

struct fmat3x3 {
public:
	float mat[3][3] = { 0.0f };
	void prnt();
};

struct fmat3x1 {
public:
	float mat[3] = { 0.0f };
	void prnt();
};

void fmat3x3mult(const fmat3x3& first, const fmat3x3& second, fmat3x3& mod);
void fmat3x3mul3x1(const fmat3x3& tf, const fmat3x1& targ, fmat3x1& mod);