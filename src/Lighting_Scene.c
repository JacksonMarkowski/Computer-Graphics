#include "CSCIx229.h"
/*
 * Basic code pieces/structure taken from CSCIx229 by Willem A. (Vlakkies) Schreuder
 */ 
int printInfo=0;
int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=40;         //  Azimuth of view angle
int ph=25;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  25;  // Ambient intensity (%)
int diffuse   =  70;  // Diffuse intensity (%)
int specular  =   5;  // Specular intensity (%)
int shininess =   1;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void ball(double x,double y,double z,double r) {
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}


//Draws a frustum with a give bottom and top radius and number of radians between each vertex
static void frustum(double x,double y,double z,
                     double r1, double r2, double height, 
                     double tx, double ty, double tz,
                     double rad) {
   double i;
   //double twoPi = 2 * M_PI;

   //Calculates the max radius for scaling
   double max = (((r1) > (r2)) ? (r1) : (r2));
   //Normalizes the two radius values
   double r1Norm = r1/max;
   double r2Norm = r2/max;

   double normalY = atan((r1-r2)/height)*2/3.1415926;

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(max,height,max);

   //Draws the frustum by drawing the next vertex rad over
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= 360; i += rad) {
      //double color = (cos(i)*cos(i) + 1) / 2;
      //glColor3f(color, color, color);
      glNormal3d(Cos(i),normalY,Sin(i));
      glVertex3d(Cos(i) * r2Norm, 1, Sin(i) * r2Norm);

      //glNormal3d(Cos(i),1,Sin(i));
      glVertex3d(Cos(i) * r1Norm, 0, Sin(i) * r1Norm);
   }
   glNormal3d(Cos(0),normalY, Sin(0));
   glVertex3d(Cos(0) * r2Norm, 1, Sin(0) * r2Norm);
   glVertex3d(Cos(0) * r1Norm, 0, Sin(0) * r1Norm);
   glEnd();

   glPopMatrix();
}

//Draws the top and bottom of a frustum
static void frustumTopBottom(double x,double y,double z,
                     double r1, double r2, double height, 
                     double tx, double ty, double tz,
                     double rad) {

   double i;
   double twoPi = 2 * M_PI;

   //Calculates the max radius for scaling
   double max = (((r1) > (r2)) ? (r1) : (r2));
   //Normalizes the two radius values
   double r1Norm = r1/max;
   double r2Norm = r2/max;

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(max,height,max);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,1,0);
   for (i = 0; i <= twoPi; i += rad) {
      glVertex3d(cos(i) * r2Norm, 1, sin(i) * r2Norm);
   }
   glVertex3d(cos(0) * r2Norm, 1, sin(0) * r2Norm);
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,-1,0);
   for (i = 0; i <= twoPi; i += rad) {
      glVertex3d(cos(i) * r1Norm, 0, sin(i) * r1Norm);
   }
   glVertex3d(cos(0) * r1Norm, 0, sin(0) * r1Norm);
   glEnd();
   

   glPopMatrix();
}

//Draws a dome, code modified from ex8
static void dome(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=0;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         glNormal3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glNormal3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   glVertex3d(Sin(0)*Cos(90) , Sin(90) , Cos(0)*Cos(90));
   for (th=0;th<=360;th+=d)
   {
      glNormal3d(0 , 1 , 0);
      glVertex3d(Sin(th)*Cos(90-d) , Sin(90-d) , Cos(th)*Cos(90-d));
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

//Draws a circular window
static void window(double x,double y, double z,
                  double dx,double dy,double dz,
                  double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   float Emission2[]  = {0.0,0.0,.2,1.0};

   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   glColor3f(.95,.95,.95);
   frustum(0, 0, 0, .6, .6, .5 , 0, 0, 0, .1);
   frustumTopBottom(0, 0, 0, .6, .6, .5 , 0, 0, 0, .1);

   glMaterialfv(GL_FRONT,GL_EMISSION,Emission2);
   glColor3f(.678,.847,.902);
   frustum(0, 0, 0, 1, 1, .2, 0, 0, 0, .1);
   frustumTopBottom(0, 0, 0, 1, 1, .2, 0, 0, 0, .1);

   glPopMatrix();
   
}

//Draws one side of the spaceship saucer
static void wing(double x,double y,double z,
                  double dx,double dy,double dz,
                  double tx)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glScaled(dx,dy,dz);

   frustum(0 ,0 , 0 , 1, .95, .25, 0, 0, 0, .05);
   frustum(0, .25, 0, .95, .85, .25, 0, 0, 0, .05);
   frustum(0, .5, 0, .85, .7, .25, 0, 0, 0, .05);
   frustum(0, .75, 0, .7, .5, .25, 0, 0, 0, .05);
   
   glPopMatrix();
}

//Draws the legs of the spaceship
static void leg(double x,double y, double z,
                  double dx,double dy,double dz,
                  double tx, double ty, double tz) {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   //Leg1
   glColor3f(.757,.757,.757);
   frustum(0, -.35, 0, 1, 1, .35, 0, 0, 0, .1);
   frustumTopBottom(0, -.35, 0, 1, 1, .35, 0, 0, 0, .1);

   //Leg2
   glColor3f(.787,.787,.787);
   frustum(0, -.85, 0, .8, .8, .5, 0, 0, 0, .1);
   frustumTopBottom(0, -.85, 0, .8, .8, .5, 0, 0, 0, .1);

   //Leg3
   glColor3f(.817,.817,.817);
   frustum(0, -1.60, 0, .55, .55, .75, 0, 0, 0, .1);
   frustumTopBottom(0, -1.60, 0, .55, .55, .75, 0, 0, 0, .1);

   glPopMatrix();

}

//Draws a spaceship at the specified cordinates
static void spaceship(double x,double y,double z,
                  double dx,double dy,double dz, 
                  double tx, double ty, double tz, int extendedLegs)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(tx,1,0,0);
   glRotated(ty,0,1,0);
   glRotated(tz,0,0,1);
   glScaled(dx,dy,dz);

   float yellow[] = {0.0,0.0,1.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   float Emission2[]  = {0.0,0.0,.2,1.0};

   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   //Top wing
   glColor3f(.827,.827,.827);
   wing(0, 0, 0, 1, .2, 1, 0);

   //dome
   dome(0,.15,0,.5);
   
   //White band between top and bottom wings
   glColor3f(.895,.895,.895);
   frustum(0, -.05, 0, 1, 1, .05, 0, 0, 0, .05);

   //Blue band between top and bottom wings
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission2);
   glColor3f(.678,.847,.902);
   frustum(0, -.0375, 0, 1.02, 1.02, .025, 0, 0, 0, .05);
   frustumTopBottom(0, -.0375, 0, 1.02, 1.02, .025, 0, 0, 0, .05);

   //Bottom wing
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   glColor3f(.735,.735,.735);
   wing(0, -.05, 0, 1, .2, 1, 180);

   //Bottom thing
   glColor3f(.672,.672,.672);
   frustum(0, -.35, 0, .4, .5, .1, 0, 0, 0, .05);
   frustumTopBottom(0, -.35, 0, .4, .5, .1, 0, 0, 0, .05);
   glColor3f(.652,.652,.652);
   frustum(0, -.425, 0, .2, .3, .075, 0, 0, 0, .05);
   frustumTopBottom(0, -.425, 0, .2, .3, .075, 0, 0, 0, .05);

   //Bottom thing blue bands
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission2);
   glColor3f(.678,.780,.871);
   frustum(0, -.27, 0, .5, .502, .02, 0, 0, 0, .05);
   frustum(0, -.37, 0, .3, .302, .02, 0, 0, 0, .05);

   
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //legs
   if (extendedLegs) {
      leg(0,-.15,.5,.05,.5,.05,-15,0,0);
      leg(.433,-.15,-.25,.05,.5,.05,0,30,15);
      leg(-.433,-.15,-.25,.05,.5,.05,0,150,15);
   }

   //Windows
   int i;
   for (i=0; i<360; i+=30) {
      window(.433 * Cos(i),.4,.433 * Sin(i),.05,.05,.05,0,-i,-60);
   }
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display() {
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //Draws vaious spaceships
   spaceship(0, 0, 0, 1, 1, 1, 0, 0, 0, 1);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   if (printInfo) {
   //  Display parameters
      glWindowPos2i(5,5);
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
        th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
      if (light)
      {
         glWindowPos2i(5,45);
         Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
         glWindowPos2i(5,25);
         Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
      }
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0') {
      th = 40;
      ph = 25;
   }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle axes
   else if (ch == 'i' || ch == 'I')
      printInfo = 1-printInfo;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(650,500);
   glutCreateWindow("Jackson Markowski");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}