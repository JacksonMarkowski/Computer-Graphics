#include "Grass.h"

Grass::Grass() {
	//loadComponents();
}

Grass::~Grass() {
	
}

void Grass::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(.63, .63, .45);
	glCallList(obj);

	glPopMatrix();
}

void Grass::loadComponents() {
	obj = LoadOBJ("../objects/grass.obj");
}