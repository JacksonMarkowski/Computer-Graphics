#include "CSCIx229.h"
#include "Ver3d.h"

//using namespace std;

class Land {
	private:
		int landObj;
		int landTex;
	public:
		Land();
		void draw();
		void loadComponents();
};

Land::Land() {
	//loadComponents();
}

void Land::draw() {
	glColor3f(1, 1, 1);
	float ambient[] = {0.7,0.7,0.7,0.7};
	float diffuse[] = {0.7,0.7,0.7,1.0};
	float specular[] = {0.7,0.7,0.7,0.0};
	float emission[] = {1.0,1.0,1.0,1.0};
	float shine = 30.0;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shine);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,landTex);
	glCallList(landObj);
	glDisable(GL_TEXTURE_2D);

}

void Land::loadComponents() {
	landObj = LoadOBJ("../objects/land.obj");
	landTex = LoadTexBMP("../textures/LandColor.bmp");
}