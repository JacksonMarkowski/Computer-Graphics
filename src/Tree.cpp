#include "CSCIx229.h"
#include "Ver3d.h"
#include "Trans3d.h"

//using namespace std;

class Tree {
	private:
		int treeObj;
		int treeTex;
		Trans3d transformation;
		void applyTrans3d(Trans3d transform);
	public:
		Tree();
		void draw();
		void loadComponents();
		void setTransformation(Trans3d transformation);
};

Tree::Tree() {
	//loadComponents();
}

void Tree::draw() {
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
	glBindTexture(GL_TEXTURE_2D,treeTex);
	glCallList(treeObj);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Tree::loadComponents() {
	treeObj = LoadOBJ("../objects/tree.obj");
	treeTex = LoadTexBMP("../textures/TreeColor.bmp");
}

void Tree::setTransformation(Trans3d transformation) {
	this->transformation = transformation;
}

void Tree::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}