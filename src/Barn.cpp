#include "Barn.h"

Barn::Barn() {
	//loadComponents();
}

Barn::~Barn() {
	
}

void Barn::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	glCallList(obj);

	glPopMatrix();
}

void Barn::loadComponents() {
	obj = LoadOBJ("../objects/barn.obj");

}