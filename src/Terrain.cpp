#include "Terrain.h"

Terrain::Terrain() {
	//loadComponents();
}

void Terrain::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);

	glCallList(obj);

	glPopMatrix();

}

void Terrain::loadComponents() {
	obj = LoadOBJ("../objects/land.obj");
}