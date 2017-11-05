#include "CSCIx229.h"

using namespace std;

struct Vec3d {
	double x;
	double y;
	double z;
	Vec3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	Vec3d() {}
	void setValues(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
};

struct Trans3d {
	Vec3d pos;
	Vec3d rot;
	Vec3d scale;
	Trans3d(double xPos, double yPos, double zPos, double xScale, double yScale, double zScale, double xRot, double yRot, double zRot) {
		pos.setValues(xPos, yPos, zPos); scale.setValues(xScale, yScale, zScale); rot.setValues(xRot, yRot, zRot);
	}
};

class Spaceship {
	private:
		void drawWingPanel(Trans3d transform);
		void drawWingPanelSideEdge(Trans3d transform, double normal);
		void drawWingPanelInsideEdge(Trans3d transform);
		void drawWingPanelOutsideEdge(Trans3d transform);
		void drawWingBase(Trans3d transform, double upToj);
		void drawDomePanel(Trans3d transform);
		void drawDomeRing(Trans3d transform);
		void drawCenterRing(Trans3d transform);
		void drawBottomOpening(Trans3d transform);
		void drawBottomOpeningRing(Trans3d transform);
		void applyTrans3d(Trans3d transform);

		void quadStripRotation(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY);
	public:
		Spaceship();
		void drawSpaceship();
};

Spaceship::Spaceship() {
	//Default Conts.
}

void Spaceship::drawSpaceship() {
	int i;
	
	for(i = 0; i < 360; i+=44) {
		drawWingPanel(Trans3d (0,0,0,1,1,1,0,i,0));
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i,0), -1);
		drawWingPanelSideEdge(Trans3d (0,0,0,1,1,1,0,i-36,0), 1);
		drawWingPanelInsideEdge(Trans3d (0,0,0,1,1,1,0,i,0));
		drawWingPanelOutsideEdge(Trans3d (0,0,0,1,1,1,0,i,0));
		drawDomePanel(Trans3d (0,.42,0,.8,.6,.8,0,i,0));
	}
	drawWingBase(Trans3d (0,-.1,0,1,1,1,0,0,0), 1.0);
	drawDomeRing(Trans3d (0,.37,0,1,1,1,0,0,0));
	drawCenterRing(Trans3d (0,-.2,0,1,1,1,0,0,0));

	drawWingBase(Trans3d (0,-.2,0,1,.5,1,180,0,0), .7);
	drawWingBase(Trans3d (0,-.35,0,.66,.5,.66,180,0,0), .8);

	drawBottomOpening(Trans3d (0,-.51,0,.41,.5,.41,180,0,0));
	drawBottomOpeningRing(Trans3d (0,-.51,0,.82,1,.82,180,0,0));
}

void Spaceship::applyTrans3d(Trans3d transform) {
	glTranslated(transform.pos.x,transform.pos.y,transform.pos.z);
	glRotated(transform.rot.x,1,0,0);
	glRotated(transform.rot.y,0,1,0);
	glRotated(transform.rot.z,0,0,1);
	glScaled(transform.scale.x,transform.scale.y,transform.scale.z);
}

void Spaceship::drawWingPanel(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);

	glColor3f(.6, .6, .6);

	double j;
	double d = .05;
   
	for (j = 0; j < 1; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotation(0,36,4.0,r1,r2,y1,y2,normalY);
	}

	glPopMatrix();
}

void Spaceship::drawWingPanelSideEdge(Trans3d transform, double normal) {
	glPushMatrix();
	applyTrans3d(transform);

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

	glPopMatrix();
}

void Spaceship::drawWingPanelInsideEdge(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);

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

	glPopMatrix();
}

void Spaceship::drawWingPanelOutsideEdge(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);

	double j = 0;
  	double r1 = 2 - j;
  	double r2 = 2.05;
  	double yVer = pow(j+1,0.5);
  	double y1 = yVer - 1;
  	double y2 = yVer - 1.11;
   
  	quadStripRotation(0,36,4.0,r1,r2,y1,y2,.705);

  	glPopMatrix();
}

void Spaceship::drawWingBase(Trans3d transform, double upToj) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.4, .4, .4);	
	double j;
	double d = .05;

	for (j = 0; j < upToj; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotation(0,360,4.0,r1,r2,y1,y2,normalY);
	}

	glPopMatrix();
}

void Spaceship::drawDomePanel(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.6, .6, .6);

	double j;
	double d = .05;

	for (j = 0; j < .9; j += d) {
		double r1 = 1 - j;
		double r2 = 1 - (j+d);
		double y1 = pow(j,0.5);
		double y2 = pow(j+d,0.5);
		double normalY = atan(2 * sqrt(j+d)) / 1.5707;
      
		quadStripRotation(0,44,4.0,r1,r2,y1,y2,normalY);

	}

	glPopMatrix();
}

void Spaceship::drawDomeRing(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.4, .4, .4);

	double r1 = 1;
	double r2 = .8;
	double r3 = 1.1;

	quadStripRotation(0,360,4.0,r1,r2,0,.05,.844);
	quadStripRotation(0,360,4.0,r1,r3,0,-.0915,.472);
   
	glPopMatrix();
}

void Spaceship::drawCenterRing(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.4, .4, .4);

	double r1 = 2;
	double r2 = 2.05;
       
	quadStripRotation(0,360,4.0,r1,r2,.1,.09,.874);
	quadStripRotation(0,360,4.0,r2,r1,.09,.05,-.57);
	quadStripRotation(0,360,4.0,r1,r2,.05,.01,.57);
	quadStripRotation(0,360,4.0,r2,r1,.01,0,-.874);
   
	glPopMatrix();
}

void Spaceship::drawBottomOpening(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.4, .4, .4);

	double j;
	double d = .05;
   
	for (j = 0; j < .2; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

		quadStripRotation(0,360,4.0,r1,r2,y1,y2,normalY);
	}

	for (j = .2; j < .8; j += d) {
  		double r1 = 2 - j;
  		double r2 = 2 - (j+d);
  		double y1 = pow(j+1,0.5)-1;
  		double y2 = pow(j+d+1,0.5)-1;
  		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;

  		double z;
  		for (z = 0; z < 360; z+=72) {
			quadStripRotation(0+z,32+z,4.0,r1,r2,y1,y2,normalY);		
		}
	}

	for (j = .8; j < 1; j += d) {
		double r1 = 2 - j;
		double r2 = 2 - (j+d);
		double y1 = pow(j+1,0.5)-1;
		double y2 = pow(j+d+1,0.5)-1;
		double normalY = atan(2 * sqrt(j+d+1)) / 1.5707;
      
		quadStripRotation(0,360,4.0,r1,r2,y1,y2,normalY);		
	}

	glPopMatrix();
}

void Spaceship::drawBottomOpeningRing(Trans3d transform) {
	glPushMatrix();
	applyTrans3d(transform);
	
	glColor3f(.4, .4, .4);

	double r1 = 1;
	double r2 = .1;
   
	quadStripRotation(0,360,4.0,r1,r2,0,.05,.96);	
   
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
}



