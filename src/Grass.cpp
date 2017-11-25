#include "Grass.h"

Grass::Grass() {
	//loadComponents();
}

void Grass::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);
	float ambient[] = {0.7,0.7,0.7,0.7};
	float diffuse[] = {0.7,0.7,0.7,1.0};
	float specular[] = {0.7,0.7,0.7,0.0};
	//float emission[] = {1.0,1.0,1.0,1.0};
	float shine = 0.0;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shine);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex);
	glCallList(obj);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Grass::loadComponents() {
	obj = LoadOBJ("../objects/grass.obj");
	tex = LoadTexBMP("../textures/GrassColor.bmp");
}