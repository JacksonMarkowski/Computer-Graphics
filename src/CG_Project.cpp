#include "CSCIx229.h"
#include "Trans3d.h"

//#include "Entity.cpp"
#include "Spaceship.cpp"
#include "Land.cpp"
#include "Barn.h"
#include "Fence.h"
#include "Grass.h"
#include "Tree.h"
#include "Cow.h"
#include "Camera.cpp"
#include "Entity.h"

int printInfo=0;
int axes=0;       //  Display axes
int move2=1;       //  Move light

int light=1;      //  Lighting

// Light values
int one       =   1;  // Unit value
int distance  =   15;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  50;  // Ambient intensity (%)
int diffuse   =  70;  // Diffuse intensity (%)
int specular  =   65;  // Specular intensity (%)
int shininess =   4;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   1.4;  // Elevation of light

int metalTex;

double currentTime = 0;
double previousTime = 0;

Spaceship mainSpaceship;
Land land;
Barn barn;
//Entity barn;
//Fence fence;
Camera camera;

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
   float Emission[]  = {0.0,0.0,(float)0.01*emission,1.0};
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


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display() {
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   camera.updateView();

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {(float)0.01*ambient ,(float)0.01*ambient ,(float)0.01*ambient ,1.0};
        float Diffuse[]   = {(float)0.01*diffuse ,(float)0.01*diffuse ,(float)0.01*diffuse ,1.0};
        float Specular[]  = {(float)0.01*specular,(float)0.01*specular,(float)0.01*specular,1.0};
        //  Light position
        float Position[]  = {(float)(distance*Cos(0)),ylight,(float)(distance*Sin(0)),1.0};
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
   double elapsedTime = currentTime - previousTime;
   mainSpaceship.setTransformation(Trans3d(0,1.5,0,.5,.5,.5,0,zh,0));
   mainSpaceship.update(elapsedTime);
   mainSpaceship.draw();

   land.setTransformation(Trans3d(0,0,0,1,1,1,0,0,0));
   land.draw();


   barn.setTransformation(Trans3d(-2.3,.95,-5,.17,.17,.17,0,10,0));
   barn.draw();

   //fence.setTransformation(Trans3d(-1.5,.95,-5,.08,.08,.08,0,0,0));
   //fence.draw();

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
      //Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
        //th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
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
   previousTime = currentTime;
   currentTime = glutGet(GLUT_ELAPSED_TIME);
   //printf("%f\n", time);
   zh = fmod(90*currentTime/1000.0,360.0);
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
      camera.incTH(5);
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      camera.incTH(-5);
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      camera.incPH(5);
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      camera.incPH(-5);
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      camera.incDIM(0.1);
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP)
      camera.incDIM(-0.1);
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

   camera.updateProjection();
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
      camera.rotate(40, 25);
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
   //else if (ch == 'p' || ch == 'P')
      //mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move2 = 1-move2;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   //else if (ch == '-' && ch>1)
      //ToDo: set fov in camera
      //fov--;
   //else if (ch == '+' && ch<179)
      //fov++;
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
   camera.updateProjection();
   //  Animate if requested
   //glutIdleFunc(move2?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   //asp = (height>0) ? (double)width/height : 1;
   camera.setASP((height>0) ? (double)width/height : 1);
      //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   camera.updateProjection();
   //Project(mode?fov:0,asp,dim);
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

   mainSpaceship.loadComponents();
   land.loadComponents();
   barn.loadComponents();
   //fence.loadComponents();

   glClearColor( .73, .913, .968, 1);
   //glEnable(GL_FOG);
   glFogf(GL_FOG_MODE, GL_LINEAR);
   GLfloat fogColor[4] = {.71f,.913f,.968f,1.0f};
   glFogfv(GL_FOG_COLOR, fogColor);
   glFogf(GL_FOG_START, 10.0f);
   glFogf(GL_FOG_END, 15.0f);

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}