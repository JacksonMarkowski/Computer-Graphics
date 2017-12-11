#include "Cow.h"

Cow::Cow() {
	//loadComponents();
}

Cow::~Cow() {
	
}

void Cow::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	glCallList(obj);

	glPopMatrix();
}

void Cow::loadComponents() {
	obj = LoadOBJ("objects/cow.obj");

}