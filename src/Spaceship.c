#include "CSCIx229.h"

static void panel(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(.6, .6, .6);

   double i, j;
   double d = 4.0, d2 = .05;
   
   for (j = 0; j < 1; j += d2) {
      double r1 = 2 - j;
      double r2 = 2 - (j+d2);

      double normalY = atan(2 * sqrt(j+d2+1)) / 1.5707;
      
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= 36; i += d) {
         
         glNormal3d(Cos(i),normalY,Sin(i));
         glVertex3d(Cos(i) * r1, pow(j+1,0.5)-1, Sin(i) * r1);
         glVertex3d(Cos(i) * r2, pow(j+d2+1,0.5)-1, Sin(i) * r2);
      }
      glEnd();
   }

   glPopMatrix();
}

static void panelSideEdge(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz, int normal) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

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
   glPopMatrix();
}

static void panelFrontEdge(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

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

static void wingBase(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(.4, .4, .4);

   double i, j;
   double d = 4.0, d2 = .05;
   
   for (j = 0; j < 1; j += d2) {
      double r1 = 2 - j;
      double r2 = 2 - (j+d2);

      double normalY = atan(2 * sqrt(j+d2+1)) / 1.5707;
      
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= 360; i += d) {
         
         glNormal3d(Cos(i),normalY,Sin(i));
         glVertex3d(Cos(i) * r1, pow(j+1,0.5)-1, Sin(i) * r1);
         glVertex3d(Cos(i) * r2, pow(j+d2+1,0.5)-1, Sin(i) * r2);
      }
      glEnd();
   }

   glPopMatrix();
}

static void wingRing(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(.4, .4, .4);

   double i;
   double d = 4.0;
   double r1 = 1;
   double r2 = .8;
   double r3 = 1.1;
   
      
   glBegin(GL_QUAD_STRIP);
   glNormal3d(0,1,0);
   for (i = 0; i <= 360; i += d) {
      glVertex3d(Cos(i) * r1, 0, Sin(i) * r1);
      glVertex3d(Cos(i) * r2, 0, Sin(i) * r2);
   }
   for (i = 0; i <= 360; i += d) {
      glNormal3d(Cos(i),.472,Sin(i));
      glVertex3d(Cos(i) * r1, 0, Sin(i) * r1);
      glVertex3d(Cos(i) * r3, -.0915, Sin(i) * r3);
   }
   glEnd();
   
   glPopMatrix();
}

static void domePanel(double x,double y,double z,
                     double dx, double dy, double dz, 
                     double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(.6, .6, .6);

   double i, j;
   double d = 4.0, d2 = .05;
   
   for (j = 0; j < .9; j += d2) {
      double r1 = 1 - j;
      double r2 = 1 - (j+d2);

      double normalY = atan(2 * sqrt(j+d2)) / 1.5707;
      
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= 44; i += d) {
         
         glNormal3d(Cos(i),normalY,Sin(i));
         glVertex3d(Cos(i) * r1, pow(j,0.5), Sin(i) * r1);
         glVertex3d(Cos(i) * r2, pow(j+d2,0.5), Sin(i) * r2);
      }
      glEnd();
   }

   glPopMatrix();
}

static void drawSpaceship() {
	int i;
	for(i = 0; i < 360; i+=44) { 
		panel(0,0,0,1,1,1,0,i,0);
		panelSideEdge(0,0,0,1,1,1,0,i,0,-1);
		panelSideEdge(0,0,0,1,1,1,0,i-36,0,1);
		panelFrontEdge(0,0,0,1,1,1,0,i,0);
		domePanel(0,.37,0,.8,.6,.8,0,i,0);
	}
	wingBase(0,-.1,0,1,1,1,0,0,0);
	wingRing(0,.37,0,1,1,1,0,0,0);
}