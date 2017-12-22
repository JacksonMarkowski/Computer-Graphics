#include "Tree.h"

Tree::Tree() {
	//loadComponents();
}

Tree::~Tree() {

}

void Tree::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	glCallList(obj);

	glPopMatrix();
}

void Tree::loadComponents() {
	obj = LoadOBJ("objects/tree.obj");
}