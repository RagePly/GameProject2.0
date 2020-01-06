#include "WorldObject.h"

Transform::Transform() : pos(0, 0, 0) {};
Transform::Transform(Float3 pos) : pos(pos) {};
Transform::~Transform() {};


void Transform::print() const {
	std::cout << "Position: "; pos.print();
}


Sphere::Sphere() : radius(0) {};
Sphere::Sphere(Shape* shape) {
	if (shape->id() == id_sphere) {
		radius = shape->getData()[0];
	}
	else {
		radius = 0;
	}
}
Sphere::Sphere(float radius) : radius(radius) {};
Sphere::~Sphere() {};

bool Sphere::intersect(const Float3& p, const Transform& tf) const {
	return tf.pos.distsq(p) <= radius * radius;
}

Float3 Sphere::normToSurf(const Float3& target, const Transform& tf) const {
	return target - tf.pos;
}

float Sphere::distToSurf(const Float3& target, const Transform& tf) const {
	return tf.pos.dist(target) - radius;
}

void Sphere::print() const {
	std::cout << 
		"Type: Sphere" << std::endl << 
		"Radius: " << radius << std::endl;
}

float* Sphere::getData() {
	return &radius;
}




Gobject::Gobject() : id(id_empty){
	shape = new EMPTY();
}

Gobject::Gobject(Shape* shape, Transform& tf)
	: id(shape->id()),
	sphere(shape),
	empty(shape)
{
	this->tf = tf;
	setupShape(shape);
}

Gobject::Gobject(Shape* shape) : id(shape->id()) {
	setupShape(shape);
}

Gobject::Gobject(Transform& tf) : id(id_empty) {
	setupShape(new EMPTY());
	this->tf = tf;
}

Gobject::~Gobject() {};

void Gobject::setupShape(Shape* shape) {
	
}

void Gobject::resetShape() {
	switch (id) {
	case id_sphere:
		this->shape = &sphere;
		break;
	default:
		break;
	}
}

bool Gobject::intersect(const Float3& point) const {
	return shape->intersect(point, tf);
}

Float3 Gobject::normToSurf(const Float3& target) const {
	return shape->normToSurf(target, tf);
}

void Gobject::print() const {
	shape->print();
	tf.print();
}

float Gobject::distToSurf(const Float3& target) const {
	return shape->distToSurf(target, tf);
}

float Gobject::SPHERE_RADI(Gobject& go) {
	if (go.id == id_sphere) {
		return go.sphere.getRadius();
	}
	else return 0.0f;
}