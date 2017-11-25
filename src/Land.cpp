#include "CSCIx229.h"
#include "Ver3d.h"
#include "Trans3d.h"

//using namespace std;

class Land {
	private:
		int landObj;
		Trans3d transformation;
		void applyTrans3d(Trans3d transform);
	public:
		Land();
		void draw();
		void loadComponents();
		void setTransformation(Trans3d transformation);
};

Land::Land() {
	//loadComponents();
}

void Land::draw() {
	glPushMatrix();
	applyTrans3d(transformation);

	glColor3f(1, 1, 1);

	glCallList(landObj);

	glPopMatrix();

}

void Land::loadComponents() {
	landObj = LoadOBJ("../objects/land.obj");
}

void Land::setTransformation(Trans3d transformation) {
	this->transformation = transformation;
}

void Land::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}