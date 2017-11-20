#include "CSCIx229.h"

//using namespace std;
#include "Ver3d.h"

struct Trans3d {
	Ver3d pos;
	Ver3d rot;
	Ver3d scale;
	Trans3d(double xPos, double yPos, double zPos, double xScale, double yScale, double zScale, double xRot, double yRot, double zRot) {
		pos.setValues(xPos, yPos, zPos); scale.setValues(xScale, yScale, zScale); rot.setValues(xRot, yRot, zRot);
	}
};

class Spaceship {
	private:
		int metal1Tex, testTex, domeRingTex, domeTex;
		void applyTrans3d(Trans3d transform);
		void drawWingPanel(Trans3d transform, int texture);
		void drawWingPanelSideEdge(Trans3d transform, int texture, double normal);
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
	public:
		Spaceship();
		void draw();
		void loadComponents();
};

Spaceship::Spaceship() {
	//metalTex = LoadTexBMP("Metal.bmp");
}

void Spaceship::draw() {
	int i;
	
	for(i = 0; i < 360; i+=45) {
		drawWingPanel(Trans3d (0,0,0,1,1,1,0,i,0), metal1Tex);
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metal1Tex, -1);
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i-36,0), metal1Tex, 1);
		drawWingPanelInsideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metal1Tex);
		drawWingPanelOutsideEdge(Trans3d (0,0,0,1,1,1,0,i,0), metal1Tex);
		drawDomePanel(Trans3d (0,.42,0,.8,.6,.8,0,i,0), domeTex);
	}
	drawWingBase(Trans3d (0,-.1,0,1,1,1,0,0,0), metal1Tex, 1.0);

	glColor4f(.768, .16, .05, .3);
	quadStripRotation(0,360,4.0,0,.081,.998,.989,.97);
	drawDomeRing(Trans3d (0,.37,0,1,1,1,0,0,0), domeRingTex);
	
	drawCenterRing(Trans3d (0,-.2,0,1,1,1,0,0,0), metal1Tex);

	drawWingBase(Trans3d (0,-.2,0,1,.5,1,180,0,0), metal1Tex, .7);
	drawWingBase(Trans3d (0,-.35,0,.66,.5,.66,180,0,0), metal1Tex, .8);

	drawBottomOpening(Trans3d (0,-.51,0,.41,.5,.41,180,0,0), metal1Tex);
	drawBottomOpeningRing(Trans3d (0,-.51,0,.82,1,.82,180,0,0), metal1Tex);

}

void Spaceship::loadComponents() {
	metal1Tex = LoadTexBMP("../textures/MetalSec.bmp");
	testTex = LoadTexBMP("../textures/Checker.bmp");
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

	glColor3f(1, 1, 1);

	glMaterialf(GL_FRONT,GL_SHININESS,128);
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

void Spaceship::drawWingPanelSideEdge(Trans3d transform, int texture, double normal) {
	glPushMatrix();
	applyTrans3d(transform);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,texture);
	glColor4f(.768, .16, .05, .3);

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

	glColor3f(1, 1, 1);

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	glColor3f(.4, .4, .4);	
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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	glColor3f(.6, .6, .6);

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	glColor3f(.6, .6, .6);

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	glColor3f(.4, .4, .4);

	double r1 = 2;
	double r2 = 2.05;
       
	quadStripRotationTex(0,360,4.0,r1,r2,.1,.09,.874,2/2.05,1);
	quadStripRotationTex(0,360,4.0,r2,r1,.09,.05,-.57,1,2/2.05);
	quadStripRotationTex(0,360,4.0,r1,r2,.05,.01,.57,2/2.05,1);
	quadStripRotationTex(0,360,4.0,r2,r1,.01,0,-.874,1,2/2.05);

	glDisable(GL_TEXTURE_2D);

	glColor4f(.768, .16, .05, .3);
	for (double i = 13.25; i < 360; i+=22.5) {
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
	
	glColor3f(.4, .4, .4);

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
  			glColor3f(.4, .4, .4);
  			glEnable(GL_TEXTURE_2D);
			quadStripRotationTex(0+z,32+z,4.0,r1,r2,y1,y2,normalY,r1/2,r2/2);
			glDisable(GL_TEXTURE_2D);

			glColor4f(.768, .16, .05, .3);
			quadStripRotation(32+z,72+z,4.0,r1,r2,y1,y2,normalY);		
		}
	}

	glColor3f(.4, .4, .4);
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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	glColor3f(.4, .4, .4);

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

