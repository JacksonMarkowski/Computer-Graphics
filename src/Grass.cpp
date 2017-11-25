#include "Grass.h"

Grass::Grass() {
	//loadComponents();
}

void Grass::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	glCallList(obj);

	glPopMatrix();
}

void Grass::loadComponents() {
	obj = LoadOBJ("../objects/grass.obj");
}