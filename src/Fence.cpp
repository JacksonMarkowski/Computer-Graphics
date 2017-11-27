#include "Fence.h"

int Fence::fenceObj;

Fence::Fence() {
	//loadComponents();
}

Fence::~Fence() {
	
}

void Fence::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	glCallList(obj);

	glPopMatrix();
}

void Fence::loadComponents() {
	if (fenceObj == 0) {
		fenceObj = LoadOBJ("../objects/fence.obj");
	}
	obj = fenceObj;

}