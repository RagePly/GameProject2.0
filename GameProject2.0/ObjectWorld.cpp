#include "ObjectWorld.h"

Transform::Transform() : pos(0, 0, 0) {};
Transform::Transform(Float3& pos) : pos(pos) {};
Transform::~Transform() {};

Float3 Transform::getPos() const { return Float3(pos); }; //TODO: check for updates where you only send a reference


