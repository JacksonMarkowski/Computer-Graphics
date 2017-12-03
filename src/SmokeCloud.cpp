#include "SmokeCloud.h"
#include <time.h>

SmokeCloud::SmokeCloud() {
	srand(time(NULL));
	pieces[0].r = (rand() % 2)/100.0 +.1;
	pieces[1].r = (rand() % 7 + 3)/100.0 + .1;
	pieces[2].r = (rand() % 10 + 5)/100.0 + .1;
	pieces[3].r = (rand() % 7 + 3)/100.0 + .1;
	pieces[4].r = (rand() % 2)/100.0 +.1;

	for (int i = 0; i < 5; i++) {
		pieces[i].y = (rand() % 10)/100.0;
		pieces[i].x = i * .2 + .1;
	}
	
}

SmokeCloud::~SmokeCloud() {

}

void SmokeCloud::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);

	float ambient[] = {0.7,0.7,0.7,0.7};
	float diffuse[] = {0.9,0.9,0.9,0.9};
	float specular[] = {0.9,0.9,0.9,0.00};
	float emission[] = {0.0,0.0,0.0,0.0};
	float shine = 5.0;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shine);

	for (int i = 0; i < 5; i++) {
		drawSmokePiece(pieces[i].x, pieces[i].y, 0, pieces[i].r);
	}

	glPopMatrix();
}

void SmokeCloud::loadComponents() {
	//ToDo: load textures
}

int SmokeCloud::update(double time) {
	elapsedTime += time;

	double timeSec = time/1000.0;

	transformation.pos.y += timeSec / 4;

	double scaleScale = elapsedTime/1000.0 > increasingTime ? -1 : 4;
	double scaleInc = scaleScale * timeSec / 10.0;
	transformation.scale.x += scaleInc;
	transformation.scale.y += scaleInc;
	transformation.scale.z += scaleInc;
	if (transformation.scale.x < .1 || transformation.scale.y < .1 || transformation.scale.z < .1) return 0;
	else return 1;
}

void SmokeCloud::drawSmokePiece(double x,double y,double z,double r) {
	int th, ph;
	int inc = 10;

	glPushMatrix();

	glTranslated(x,y,z);
	glScaled(r,r,r);

	for (ph = -90; ph < 90; ph += inc) {
		glBegin(GL_QUAD_STRIP);
		for (th=0; th<=360; th += 2*inc) {
			Vertex(th,ph);
			Vertex(th,ph+inc);
		}
		glEnd();
	}
	glPopMatrix();
}

void SmokeCloud::Vertex(double th,double ph) {
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);

	glNormal3d(x,y,z);
	glVertex3d(x,y,z);
}