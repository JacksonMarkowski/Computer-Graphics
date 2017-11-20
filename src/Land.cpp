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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,landTex);
	glCallList(landObj);
	glDisable(GL_TEXTURE_2D);

}

void Land::loadComponents() {
	landObj = LoadOBJ("../objects/land.obj");
	landTex = LoadTexBMP("../textures/LandColor.bmp");
}