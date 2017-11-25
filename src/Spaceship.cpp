#include "CSCIx229.h"

//using namespace std;
#include "Ver3d.h"

#include "Trans3d.h"

struct light {
	double value;
	int brightening;
};

class Spaceship {
	private:
		int metalSecTex, domeRingTex, domeTex;
		Trans3d transformation;
		struct light lights[8];
		void applyTrans3d(Trans3d transform);

		void drawWingPanel(Trans3d transform, int texture);
		void drawWingPanelSideEdge(Trans3d transform, int texture, double normal, double emission);
		void drawWingPanelInsideEdge(Trans3d transform, int texture);
		void drawWingPanelOutsideEdge(Trans3d transform, int texture);
		void drawWingBase(Trans3d transform, int texture, double upToj);
		void drawDomePanel(Trans3d transform, int texture);
		void drawDomeRing(Trans3d transform, int texture);
		void drawCenterRing(Trans3d transform, int texture);
		void drawBottomOpening(Trans3d transform, int texture);
		void drawBottomOpeningRing(Trans3d transform, int texture);

		void quadStripRotation(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY);
		void quadStripRotationTex(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY, double r1Tex, double r2Tex);
		
		void applyMetalMaterial();
		void applyLightMaterial();
	public:
		Spaceship();
		void setTrans3d(Trans3d transformation);
		void update(double time);
		void draw();
		void loadComponents();
};

Spaceship::Spaceship() {
	//metalTex = LoadTexBMP("Metal.bmp");
	lights[0].value = 1.0; lights[0].brightening = 0;
	lights[1].value = .75; lights[1].brightening = 1;
	lights[2].value = .5; lights[2].brightening = 1;
	lights[3].value = .25; lights[3].brightening = 1;
	lights[4].value = .0; lights[4].brightening = 1;
	lights[5].value = .25; lights[5].brightening = 0;
	lights[6].value = .5; lights[6].brightening = 0;
	lights[7].value = .75; lights[7].brightening = 0;

}

void Spaceship::setTrans3d(Trans3d transformation) {
	this->transformation = transformation;
}

void Spaceship::update(double elapsedTime) {
	
	double lightChange = elapsedTime/750.0;
	for (int i = 0; i < 8; i++) {
		double remainingChange = lightChange;
		while (remainingChange != 0) {
			if (lights[i].brightening) {
				lights[i].value += remainingChange;
				if (lights[i].value >= 1.0) {
					remainingChange = lights[i].value - 1;
					lights[i].value = 1.0;
					lights[i].brightening = 0;
				} else remainingChange = 0;
			} else {
				lights[i].value -= remainingChange;
				if (lights[i].value <= 0.0) {
					remainingChange = fabs(lights[i].value);
					lights[i].value = 0.0;
					lights[i].brightening = 1;
				} else remainingChange = 0;
			}
		}
	}
	//lightOnDeg += fmod(360*elapsedTime/1000.0,360.0);
	//lightOnDeg %= 360;

}

void Spaceship::draw() {
	int i;
	glPushMatrix();
	applyTrans3d(transformation);
	
	for(i = 0; i < 360; i+=45) {
		drawWingPanel(Trans3d (0,0,0,1,1,1,0,i,0), metalSecTex);
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metalSecTex, -1, lights[i/45].value);
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i-36,0), metalSecTex, 1, lights[i/45].value);
		drawWingPanelInsideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metalSecTex);
		drawWingPanelOutsideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metalSecTex);
		drawDomePanel(Trans3d (0,.42,0,.8,.6,.8,0,i,0), domeTex);
	}
	drawWingBase(Trans3d (0,-.1,0,1,1,1,0,0,0), metalSecTex, 1.0);

	glColor4f(.768, .16, .05, .8);
	quadStripRotation(0,360,4.0,0,.081,.998,.989,.97);
	drawDomeRing(Trans3d (0,.37,0,1,1,1,0,0,0), domeRingTex);
	
	drawCenterRing(Trans3d (0,-.2,0,1,1,1,0,0,0), metalSecTex);

	drawWingBase(Trans3d (0,-.2,0,1,.5,1,180,0,0), metalSecTex, .7);
	drawWingBase(Trans3d (0,-.35,0,.66,.5,.66,180,0,0), metalSecTex, .8);

	drawBottomOpening(Trans3d (0,-.51,0,.41,.5,.41,180,0,0), metalSecTex);
	drawBottomOpeningRing(Trans3d (0,-.51,0,.82,1,.82,180,0,0), metalSecTex);

	glPopMatrix();

}

void Spaceship::loadComponents() {
	metalSecTex = LoadTexBMP("../textures/MetalSec.bmp");
	domeRingTex = LoadTexBMP("../textures/DomeRing.bmp");
	domeTex = LoadTexBMP("../textures/MetalSecDome.bmp");
}

void Spaceship::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}

void Spaceship::drawWingPanel(Trans3d transform, int texture) {
	glPushMatrix();
	double startDegree = transform.rot.y;
	transform.rot.y = 0;
	applyTrans3d(transform);

	glColor3f(.65, .65, .65);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double j;
	double d = .05;
   
	for (j = 0; j < 1; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotationTex(startDegree,startDegree+36,4.0,r1,r2,y1,y2,normalY, r1/2, r2/2);
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawWingPanelSideEdge(Trans3d transform, int texture, double normal, double emission) {
	glPushMatrix();
	applyTrans3d(transform);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,texture);
	//glColor4f(.494+(emission*.404), .1, .1, .8);
	if (emission > .7) glColor4f(1+(emission*.6), .1, .1, .8);
	else glColor4f(.494, .1, .1, .8);

	applyLightMaterial();

	double j;
	double d2 = .05;
	
	glBegin(GL_QUAD_STRIP);
	glNormal3d(0,0,normal);
	for (j = 0; j <= 1.05; j += d2) {
		double yVer = pow(j+1,0.5);

		glVertex3d(2-j, yVer-1, 0);
		glVertex3d(2-j, yVer-1.1, 0);

	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3d(2, 0, 0);
	glVertex3d(2, -.1, 0);
	glVertex3d(2.05, -.11, 0);
	glEnd();

	//glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawWingPanelInsideEdge(Trans3d transform, int texture) {
	glPushMatrix();
	applyTrans3d(transform);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,texture);

	double i;
	double j = 1;
	double d = 4.0;

	double r2 = 2 - j;
	double yVer = pow(j+1,0.5);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= 36; i += d) {
         
		glNormal3d(-Cos(i),0,-Sin(i));
		glVertex3d(Cos(i) * r2, yVer-1, Sin(i) * r2);
		glVertex3d(Cos(i) * r2, yVer-1.1, Sin(i) * r2);
	}
	glEnd();

	//glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawWingPanelOutsideEdge(Trans3d transform, int texture) {
	glPushMatrix();
	double startDegree = transform.rot.y;
	transform.rot.y = 0;
	applyTrans3d(transform);

	glColor3f(.6, .6, .6);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double j = 0;
  	double r1 = 2 - j;
  	double r2 = 2.05;
  	double yVer = pow(j+1,0.5);
  	double y1 = yVer - 1;
  	double y2 = yVer - 1.11;
   
  	quadStripRotationTex(startDegree,startDegree+36,4.0,r1,r2,y1,y2,.705, r1/2, r2/2);

  	glDisable(GL_TEXTURE_2D);

  	glPopMatrix();
}

void Spaceship::drawWingBase(Trans3d transform, int texture, double upToj) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.5, .5, .5);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double j;
	double d = .05;

	for (j = 0; j < upToj; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotationTex(0,360,4.0,r1,r2,y1,y2,normalY,r1/2,r2/2);
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawDomePanel(Trans3d transform, int texture) {
	glPushMatrix();
	double startDegree = transform.rot.y;
	transform.rot.y = 0;
	applyTrans3d(transform);

	glColor3f(.6, .6, .6);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double j;
	double d = .05;

	for (j = 0; j < .9; j += d) {
		double r1 = 1 - j;
		double r2 = 1 - (j+d);
		double y1 = pow(j,0.5);
		double y2 = pow(j+d,0.5);
		double normalY = atan(2 * sqrt(j+d)) / 1.5707;
      
		quadStripRotationTex(startDegree,startDegree+45,5.0,r1,r2,y1,y2,normalY,r1,r2);
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawDomeRing(Trans3d transform, int texture) {
	glPushMatrix();
	applyTrans3d(transform);

	glColor3f(.6, .6, .6);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double r1 = 1;
	double r2 = .8;
	double r3 = 1.1;

	quadStripRotationTex(0,360,4.0,r1,r2,0,.05,.844,1,.83);
	
	glBindTexture(GL_TEXTURE_2D,texture);
	quadStripRotationTex(0,360,4.0,r3,r1,-.0915,0,.472,1,.9);

	glDisable(GL_TEXTURE_2D);
   
	glPopMatrix();
}

void Spaceship::drawCenterRing(Trans3d transform, int texture) {
	glPushMatrix();
	applyTrans3d(transform);

	glColor3f(.5, .5, .5);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double r1 = 2;
	double r2 = 2.05;
       
	quadStripRotationTex(0,360,4.0,r1,r2,.1,.09,.874,2/2.05,1);
	quadStripRotationTex(0,360,4.0,r2,r1,.09,.05,-.57,1,2/2.05);
	quadStripRotationTex(0,360,4.0,r1,r2,.05,.01,.57,2/2.05,1);
	quadStripRotationTex(0,360,4.0,r2,r1,.01,0,-.874,1,2/2.05);

	glDisable(GL_TEXTURE_2D);

	//glColor4f(.768, .16, .05, .6);
	for (double i = 13.25; i < 360; i+=22.5) {
		double emission = lights[((int)i)/45].value;
		if (emission > .7) glColor4f(1+(emission*.6), .1, .1, .8);
		else glColor4f(.494, .1, .1, .8);
		for (double j = 0; j <= 8; j += 4) {
			quadStripRotation(i+j,i+j+1.5,.75,2.0125,2.0125,.06,.04,0);
			/*
			glBegin(GL_TRIANGLES);
			glVertex3d(Cos(i+j)*2.0125, .06, Sin(i+j)*2.0125);
			glVertex3d(Cos(i+j)*2.0125, .04, Sin(i+j)*2.0125);
			glVertex3d(Cos(i+j)*2, .05, Sin(i+j)*2);
			glEnd();*/
		}
	}
	glDisable(GL_TEXTURE_2D);
   
	glPopMatrix();
}

void Spaceship::drawBottomOpening(Trans3d transform, int texture) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.5, .5, .5);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double j;
	double d = .05;
   
	for (j = 0; j < .2; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotationTex(0,360,4.0,r1,r2,y1,y2,normalY,r1/2,r2/2);
	}

	for (j = .2; j < .8; j += d) {
  		double r1 = 2 - j;
  		double r2 = 2 - (j+d);
  		double y1 = pow(j+1,0.5)-1;
  		double y2 = pow(j+d+1,0.5)-1;
  		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

  		double z;
  		for (z = 0; z < 360; z+=72) {
  			glColor3f(.5, .5, .5);
			applyMetalMaterial();
  			glEnable(GL_TEXTURE_2D);
			quadStripRotationTex(0+z,32+z,4.0,r1,r2,y1,y2,normalY,r1/2,r2/2);
			glDisable(GL_TEXTURE_2D);

			glColor4f(.494, .1, .1, .8);
			quadStripRotation(32+z,72+z,4.0,r1,r2,y1,y2,normalY);		
		}
	}

	glColor3f(.5, .5, .5);
	applyMetalMaterial();
	glEnable(GL_TEXTURE_2D);
	for (j = .8; j < 1; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;
      
		quadStripRotationTex(0,360,4.0,r1,r2,y1,y2,normalY,r1/2,r2/2);		
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Spaceship::drawBottomOpeningRing(Trans3d transform, int texture) {
	glPushMatrix();
	applyTrans3d(transform);

	glColor3f(.5, .5, .5);
	applyMetalMaterial();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);

	double r1 = 1;
	double r2 = .1;
   
	quadStripRotationTex(0,360,4.0,r1,r2,0,.05,.96,r1,r2);	

	glDisable(GL_TEXTURE_2D);
   
	glPopMatrix();
}

void Spaceship::quadStripRotation(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY) {
	double i;

	glBegin(GL_QUAD_STRIP);
	for (i = startDegree; i <= endDegree; i += incDegree) {
		glNormal3d(Cos(i),normalY,Sin(i));
		glVertex3d(Cos(i) * r1, y1, Sin(i) * r1);
		glVertex3d(Cos(i) * r2, y2, Sin(i) * r2);
	}
	glEnd();
	glTexCoord2f(i/360, 1.0);
}

void Spaceship::quadStripRotationTex(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY, double r1Tex, double r2Tex) {
	double i;

	glBegin(GL_QUAD_STRIP);
	for (i = startDegree; i <= endDegree; i += incDegree) {
		glNormal3d(Cos(i),normalY,Sin(i));
		glTexCoord2f((Cos(i) * r1Tex / 2.0 + .5), (Sin(i) * r1Tex / 2.0 + .5)); glVertex3d(Cos(i) * r1, y1, Sin(i) * r1);
		glTexCoord2f((Cos(i) * r2Tex / 2.0 + .5), (Sin(i) * r2Tex / 2.0 + .5)); glVertex3d(Cos(i) * r2, y2, Sin(i) * r2);
	}
	glEnd();
}

void Spaceship::applyMetalMaterial() {
	float ambient[] = {0.6,0.6,0.6,0.7};
	float diffuse[] = {0.6,0.6,0.6,0.2};
	float specular[] = {0.6,0.6,0.6,1.0};
	float emission[] = {0.0,0.0,0.0,0.0};
	float shine = 30.0;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shine);
}

void Spaceship::applyLightMaterial() {
	float ambient[] = {.494, .1, .1, .05};
	float diffuse[] = {.494, .1, .1, .05};
	float specular[] = {.494, .1, .1, .2};
	float emission[] = {0, 0, 0, 1};
	//float shine = 30.0;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emission);
	//glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shine);
}

