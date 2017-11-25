#include "Entity.h"

Entity::Entity() {
	//loadComponents();
}

void Entity::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);

	glCallList(obj);

	glPopMatrix();
}

void Entity::loadComponents() {
	//default obj
	//obj = LoadOBJ("../objects/Null.obj");
}


void Entity::update(double elapsedTime) {
	//Update entity
}

void Entity::setTrans3d(Trans3d transformation) {
	this->transformation = transformation;
}

void Entity::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}