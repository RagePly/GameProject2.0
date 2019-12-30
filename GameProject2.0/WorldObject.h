#pragma once
#include "Geometry.h"


class Transform {
public:
	Transform();
	Transform(Float3 pos);
	~Transform();

	Float3 pos;
	void print() const;
private:
	//TODO: check the ability to have a private backup of the position
	//that way you need to call an update to the transform to move, 
	//but can easily access the position
	
};

class Shape {
public:
	virtual bool intersect(const Float3& point, const Transform& tf) const = 0;
	virtual Float3 normToSurf(const Float3& target, const Transform& tf) const = 0; //The target indicates at which side the normal is on;
	virtual float distToSurf(const Float3& target, const Transform& tf) const = 0;
	virtual void print() const = 0;
	virtual float* getData() = 0;
	virtual int id() const = 0;
};



class EMPTY : public Shape {
public:
	EMPTY() {};
	EMPTY(Shape* shape) {};
	~EMPTY() {};

	bool intersect(const Float3& point, const Transform& tf) const { return false; };
	Float3 normToSurf(const Float3& target, const Transform& tf) const { return Float3(0, 0, 0); };
	float distToSurf(const Float3& target, const Transform& tf) const { return tf.pos.dist(target); };
	void print() const {
		std::cout << "Type: EMPTY" << std::endl;
	};
	float* getData() { return nullptr; }; 
	int id() const { return id_empty; };
};


class Sphere : public Shape {
public:
	Sphere();
	Sphere(Shape* shape);
	Sphere(float radius);
	~Sphere();

	bool intersect(const Float3& point, const Transform& tf) const;
	Float3 normToSurf(const Float3& target, const Transform& tf) const;
	float distToSurf(const Float3& tarfet, const Transform& tf) const;
	void print() const;
	float* getData();
	int id() const { return id_sphere; };
	float getRadius() const { return radius; };
private:
	float radius;
};


class Gobject {
public:
	Gobject();
	Gobject(Shape* shape, Transform& tf);
	Gobject(Shape* shape);
	Gobject(Transform& tf);
	~Gobject();

	static float SPHERE_RADI(Gobject& go);

	bool intersect(const Float3& point) const;
	Float3 normToSurf(const Float3& target) const;
	float distToSurf(const Float3& target) const;
	void print() const;
	Transform tf;
	Shape* shape;
	RGB_COLOR color;
	void resetShape();
	int id;
private:
	void setupShape(Shape* shape);
	EMPTY empty;
	Sphere sphere;
	
	//TODO: SIMILAR COMMENT THAT APPEARED AT TRANSFORM
};





