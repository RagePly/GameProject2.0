#pragma once
#include "Geometry.h"


class Gobject {
public:
	Gobject();
	Gobject(Shape* shape, Transform& tf);
	Gobject(Shape* shape);
	Gobject(Transform& tf);
	~Gobject();

	void intersect(const Float3& point) const;
private:
	Transform* tf;
	Shape* shape;
};

class Shape {
public:
	virtual bool intersect(const Float3& point, const Transform& tf) const = 0;
};

class EMPTY : public Shape {
public:
	EMPTY() {};
	~EMPTY() {};

	bool intersect(const Float3& point, const Transform& tf) const { return false; };
};


class Sphere : public Shape {
public:
	Sphere();
	Sphere(float radius);
	~Sphere();

	bool intersect(const Float3& point, const Transform& tf) const;
private:
	float radius;
};

class Transform {
public:
	Transform();
	Transform(Float3& pos);
	~Transform();

	Float3 getPos() const; //might cause problems
private:
	Float3 pos;
};

