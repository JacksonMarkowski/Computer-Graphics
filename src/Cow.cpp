#include "CSCIx229.h"
#include "Ver3d.h"
#include "Trans3d.h"

//using namespace std;

class Cow {
	private:
		int cowObj;
		int cowTex;
		Trans3d transformation;
		void applyTrans3d(Trans3d transform);
	public:
		Cow();
		void draw();
		void loadComponents();
		void setTransformation(Trans3d transformation);
};

Cow::Cow() {
	//loadComponents();
}

void Cow::draw() {
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
	glBindTexture(GL_TEXTURE_2D,cowTex);
	glCallList(cowObj);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Cow::loadComponents() {
	cowObj = LoadOBJ("../objects/cow.obj");
	cowTex = LoadTexBMP("../textures/CowColor.bmp");
}

void Cow::setTransformation(Trans3d transformation) {
	this->transformation = transformation;
}

void Cow::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}