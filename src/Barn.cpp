#include "CSCIx229.h"
#include "Ver3d.h"
#include "Trans3d.h"

//using namespace std;

class Barn {
	private:
		int barnObj;
		int barnTex;
		Trans3d transformation;
		void applyTrans3d(Trans3d transform);
	public:
		Barn();
		void draw();
		void loadComponents();
		void setTransformation(Trans3d transformation);
};

Barn::Barn() {
	//loadComponents();
}

void Barn::draw() {
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
	glBindTexture(GL_TEXTURE_2D,barnTex);
	glCallList(barnObj);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Barn::loadComponents() {
	barnObj = LoadOBJ("../objects/barn.obj");
	barnTex = LoadTexBMP("../textures/BarnColor.bmp");
}

void Barn::setTransformation(Trans3d transformation) {
	this->transformation = transformation;
}

void Barn::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}