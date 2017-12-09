#include "CSCIx229.h"
#include "Trans3d.h"
#include <list>

#include "Entity.h"
#include "Spaceship.h"
#include "SmokeCloud.h"
#include "Terrain.h"
#include "Barn.h"
#include "Fence.h"
#include "Grass.h"
#include "Tree.h"
#include "Cow.h"
#include "Camera.cpp"

int printInfo=0;  //  Print info
int axes=0;       //  Display axes
int light=1;      //  Lighting
int beamLight=1;
int dayTime = 1;
int viewMode = 2;

// Light values
int distance  =   15;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
float ambient   =  50;  // Ambient intensity (%)
float diffuse   =  70;  // Diffuse intensity (%)
float specular  =   65;  // Specular intensity (%)
float ylight  =   1.4;  // Elevation of light

double currentTime = 0;
double elapsedTime = 0;

double elapsedSmokeGeneration = 0;

int inTimeOfDayTransition = 0;

int inScene = 0;
int currentScene = 0;
double currentSceneTime = 0;

std::list<Entity*> entities;
std::list<SmokeCloud*> smokeClouds;

Spaceship mainSpaceship;

Cow abductedCow;

Terrain land;
Camera camera;

void enableFog(GLfloat* fogColor);
void toggleTimeOfDay();

struct SkyValues {
	GLclampf skyColor[4];
	GLfloat fogColor[4];
	float ambientSky;
	float diffuseSky;
	float specularSky;
	SkyValues() {}
	void setValues(GLclampf skyRed, GLclampf skyGreen, GLclampf skyBlue, GLclampf skyAlpha, GLfloat fogRed, GLfloat fogGreen, GLfloat fogBlue, GLfloat fogAlpha, int ambient, int diffuse, int specular) {
		skyColor[0] = skyRed; skyColor[1] = skyGreen; skyColor[2] = skyBlue; skyColor[3] = skyAlpha;
		fogColor[0] = fogRed; fogColor[1] = fogGreen; fogColor[2] = fogBlue; fogColor[3] = fogAlpha;
		ambientSky = ambient; diffuseSky = diffuse; specularSky = specular;
	}
	void applyValues() {
		glClearColor( skyColor[0], skyColor[1], skyColor[2], skyColor[3]);
		enableFog(fogColor); 
		ambient = ambientSky;
		diffuse = diffuseSky;
		specular = specularSky;
	}
};

SkyValues dayTimeSky;
SkyValues nightTimeSky;
SkyValues currentTimeSky;

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph) {
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
	glMaterialf(GL_FRONT,GL_SHININESS,100);
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	//  Bands of latitude
	for (ph=-90;ph<90;ph+=inc) {
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=2*inc) {
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
	camera.updateView(mainSpaceship.getPos());

	//  Flat or smooth shading
	glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

	//  Light switch
	
	if (light) {
		//  Translate intensity to color vectors
		float Ambient[]   = {(float)0.01*ambient ,(float)0.01*ambient ,(float)0.01*ambient ,1.0};
		float Diffuse[]   = {(float)0.01*diffuse ,(float)0.01*diffuse ,(float)0.01*diffuse ,1.0};
		float Specular[]  = {(float)0.01*specular,(float)0.01*specular,(float)0.01*specular,1.0};
		//  Light position
		float Position[]  = {(float)(distance*Cos(45)),ylight,(float)(distance*Sin(45)),1.0};
		//  Draw light position as ball (still no lighting here)
		glColor3f(1,1,1);
		ball(Position[0],Position[1],Position[2] , 0.15);
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
	} else {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}; 

	mainSpaceship.update(elapsedTime);
	mainSpaceship.draw();

	land.draw();

	abductedCow.draw();

	for (std::list<SmokeCloud*>::iterator it = smokeClouds.begin(); it != smokeClouds.end(); ++it) {
		if (!(*it)->update(elapsedTime)) {
			delete * it;  
			it = smokeClouds.erase(it);
			(*it)->update(elapsedTime);
			(*it)->draw();
		} else (*it)->draw();
	}

	for (Entity* e : entities) {
		e->update(elapsedTime);
		e->draw();
	}

	mainSpaceship.drawBeam(camera.getHorRotFromCamPos(mainSpaceship.getPos()));

	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	if (axes) {
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
		if (light) {
			glWindowPos2i(5,45);
			Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
			glWindowPos2i(5,25);
			Print("Ambient=%d  Diffuse=%d Specular=%d",ambient,diffuse,specular);
		}
	}

	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}

void generateSmokeClouds() {
	double timeBetweenGeneration = 800.0;
	elapsedSmokeGeneration += elapsedTime;
	while (elapsedSmokeGeneration > timeBetweenGeneration) {
		srand(time(NULL));
		double x = ((rand() % 3) - 1)/5.0;
		double z = ((rand() % 3) - 1)/7.0;
		double ry = (rand() % 360);
		double rz = (rand() % 38);
		SmokeCloud* cloud = new SmokeCloud();

		cloud->setTrans3d(Trans3d(.32+x,.9,-1.7+z,.11,.11,.11,0,ry,rz));

		smokeClouds.push_back(cloud);

		elapsedSmokeGeneration -= timeBetweenGeneration;
	}
}

void handleTimeOfDayTransition() {
	double incValue = elapsedTime/2500.0;
	
	//int count = 0;

	SkyValues toSky;
	SkyValues fromSky;
	if (dayTime) {
		toSky = dayTimeSky;
		fromSky = nightTimeSky;
	} else {
		toSky = nightTimeSky;
		fromSky = dayTimeSky;
	}
	for (int i = 0; i < 4; i++) {
		currentTimeSky.skyColor[i] += incValue * (toSky.skyColor[i] - fromSky.skyColor[i]);
		currentTimeSky.fogColor[i] += incValue * (toSky.fogColor[i] - fromSky.fogColor[i]);

		if ((currentTimeSky.skyColor[i] > toSky.skyColor[i] && currentTimeSky.skyColor[i] > fromSky.skyColor[i]) || (currentTimeSky.skyColor[i] < toSky.skyColor[i] && currentTimeSky.skyColor[i] < fromSky.skyColor[i])) {
			currentTimeSky = toSky;
			inTimeOfDayTransition = 0;
			currentTimeSky.applyValues();
			return;
		}
		if ((currentTimeSky.fogColor[i] > toSky.fogColor[i] && currentTimeSky.fogColor[i] > fromSky.fogColor[i]) || (currentTimeSky.fogColor[i] < toSky.fogColor[i] && currentTimeSky.fogColor[i] < fromSky.fogColor[i])) {
			currentTimeSky = toSky;
			inTimeOfDayTransition = 0;
			currentTimeSky.applyValues();
			return;
		}
	}
	currentTimeSky.ambientSky += incValue * (toSky.ambientSky - fromSky.ambientSky);
	currentTimeSky.diffuseSky += incValue * (toSky.diffuseSky - fromSky.diffuseSky);
	currentTimeSky.specularSky += incValue * (toSky.specularSky - fromSky.specularSky);
	currentTimeSky.applyValues();
}

void handleScenes() {
	currentSceneTime += elapsedTime;
	int enterScene = currentScene;
	switch (currentScene) {
		case 0:
			dayTime = 1;
			currentTimeSky = dayTimeSky;
			dayTimeSky.applyValues();

			camera.setViewMode(Camera::FOLLOW);
			camera.setCamPos(4.50,2.90,2.898076);

			toggleTimeOfDay();

			mainSpaceship.setPos(Ver3d(-13.1,3.2,-.3));
			mainSpaceship.setBeamOnOff(0);
			beamLight = 0;

			abductedCow.setTrans3d(Trans3d(2.2,-.3,-.2,.075,.075,.075,13,11,0));

			currentScene++;
			break;
		case 1:
			mainSpaceship.incX((elapsedTime / 1000) * 3.04);
			if (currentSceneTime >= 3000) currentScene++;
			break;
		case 2:
			mainSpaceship.incY((elapsedTime / 1000) * -1.0);
			mainSpaceship.incX((elapsedTime / 1000) * 3.02);
			if (currentSceneTime >= 2000) currentScene++;
			break;
		case 3:
			mainSpaceship.setBeamOnOff(1);
			currentScene++;
			break;
		case 4:
			if (currentSceneTime >= 400) currentScene++;
			break;
		case 5: {
			abductedCow.incY((elapsedTime / 1000) * .44);
			double scaleInc = (elapsedTime / 2500) * -.042;
			abductedCow.incScale(scaleInc, scaleInc, scaleInc);
			if (currentSceneTime >= 2800) currentScene++;
			break;
		}
		case 6: {
			double incY = (elapsedTime / 1000) * 0.2;
			double incZ = (elapsedTime / 1000) * -0.8;
			mainSpaceship.incY(incY);
			mainSpaceship.incZ(incZ);
			abductedCow.incY(incY);
			abductedCow.incZ(incZ);
			if (currentSceneTime >= 1200) currentScene++;
			break;
		}
		case 7:
			mainSpaceship.setBeamOnOff(0);
			currentScene++;
			break;
		case 8: {
			double incX = (elapsedTime / 1000) * -0.5;
			double incY = (elapsedTime / 1000) * 0.3;
			double incZ = (elapsedTime / 1000) * -2.5;
			mainSpaceship.incX(incX);
			mainSpaceship.incY(incY);
			mainSpaceship.incZ(incZ);
			abductedCow.incX(incX);
			abductedCow.incY(incY);
			abductedCow.incZ(incZ);
			if (currentSceneTime >= 5000) currentScene++;
			break;
		}
		case 9:
			if (currentSceneTime >= 1000) currentScene++;
			break;
		case 10:
			mainSpaceship.setTrans3d(Trans3d(0,1.5,0,.5,.5,.5,0,0,0));
			mainSpaceship.setBeamOnOff(1);
			beamLight = 1;

			abductedCow.setTrans3d(Trans3d(2.2,-.3,-.2,.075,.075,.075,13,11,0));
			inScene = 0;
			break;
	}
	if (currentScene != enterScene) currentSceneTime = 0;
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle() {

	double previousTime = currentTime;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;

	generateSmokeClouds();
	if (inTimeOfDayTransition) handleTimeOfDayTransition();
	if (inScene) handleScenes();

	glutPostRedisplay();
}

void enableFog(GLfloat* fogColor) {
	glEnable(GL_FOG);
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	//glFogf(GL_FOG_COORD_SRC, GL_FOG_COORD);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 14.0f);
}

void toggleTimeOfDay() {
	dayTime = 1-dayTime;
	inTimeOfDayTransition = 1;
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y) {
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

	camera.updateProjection();
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y) {
	//  Exit on ESC
	if (ch == 27)
		exit(0);
	//Toggle axes
	else if (ch == 'x' || ch == 'X')
		axes = 1-axes;
	//Toggle axes
	else if (ch == 'i' || ch == 'I')
		printInfo = 1-printInfo;
	//Toggle main light(sun/moon)
	else if (ch == 'l' || ch == 'L')
		light = 1-light;
	//Switch projection mode
	else if (ch == 'p' || ch == 'P') {
		int viewMode = camera.getViewMode();
		viewMode++;
		viewMode %= 3;
		if (viewMode == 0) {
			camera.setViewMode(Camera::FIRST_PERSON);
			camera.rotate(-240,-30);
		}
		else if (viewMode == 1) camera.setViewMode(Camera::FOLLOW);
		else if (viewMode == 2) {
			camera.setViewMode(Camera::PERSPECTIVE);
			camera.rotate(-60,30);
		}
	}
	/*
	//Change field of view angle
	else if (ch == '-' && ch>1)
		//ToDo: set fov in camera
		fov--;
	else if (ch == '+' && ch<179)
		fov++;
	*/
	//Move the spaceship forwards
	else if (ch == 'w') {
		mainSpaceship.incX(-.2);
	}
	//Move the spaceship left
	else if (ch == 'a') {
		mainSpaceship.incZ(.2);
	}
	//Move the spaceship right
	else if (ch == 'd') {
		mainSpaceship.incZ(-.2);
	}
	//Move the spaceship backwards
	else if (ch == 's') {
		mainSpaceship.incX(.2);
	}
	//Move the spaceship up
	else if (ch == 'e' || ch == 'E') {
		mainSpaceship.incY(.1);
	}
	//Mode the spaceship down
	else if (ch == 'q' || ch == 'Q') {
		mainSpaceship.incY(-.1);
	}
	//Move camera position forwards
	else if (ch == 'W') {
		camera.moveCamPosForward(.2);
	}
	//Move camera position left
	else if (ch == 'A') {
		camera.moveCamPosSideways(.2);
	}
	//Move camera position right
	else if (ch == 'D') {
		camera.moveCamPosSideways(-.2);
	}
	//Move camera position backwards
	else if (ch == 'S') {
		camera.moveCamPosForward(-.2);
	}
	else if (ch=='b' || ch=='B') {
		beamLight = 1-beamLight;
		mainSpaceship.setBeamOnOff(beamLight);
	}
	//Toggle the time of day(night or day)
	else if (ch=='t' || ch=='T') {
		toggleTimeOfDay();
	}
	else if (ch==32) {
		inScene = 1;
		currentScene = 0;
	}

	camera.updateProjection();
	glutPostRedisplay();
}

void reshape(int width,int height) {
	camera.setASP((height>0) ? (double)width/height : 1);
	glViewport(0,0, width,height);
	camera.updateProjection();
}

void newEntity(Entity* e, Trans3d trans) {
	e->loadComponents();
	e->setTrans3d(trans);
	entities.push_back(e);
}

void generateEntities() {
	//ToDo: add spaceship and land to entities list
	mainSpaceship.setBeamOnOff(1);
	mainSpaceship.loadComponents();
	mainSpaceship.setTrans3d(Trans3d(0,1.5,0,.5,.5,.5,0,0,0));

	land.loadComponents();
	land.setTrans3d(Trans3d(0,0,0,1,1,1,0,0,0));

	newEntity(new Spaceship(0), Trans3d(.35,.56,-1.7,.45,.45,.45,-40,0,155));

	newEntity(new Barn(), Trans3d(-2.3,.95,-5,.17,.17,.17,0,10,0));

	newEntity(new Fence(), Trans3d(-1.76,.95,-6,.08,.08,.08,0,0,-5.0));
	newEntity(new Fence(), Trans3d(-.671,.863,-6,.08,.08,.08,0,0,-11.9));
	newEntity(new Fence(), Trans3d(.462,.62,-6,.08,.08,.08,0,0,-14.2));
	newEntity(new Fence(), Trans3d(1.55,.34,-6,.08,.08,.08,0,0,-13));
	newEntity(new Fence(), Trans3d(2.689,.07,-6,.08,.08,.08,0,-45,-5));
	newEntity(new Fence(), Trans3d(3.50,-.055,-5.15,.08,.08,.08,0,-90,0));
	newEntity(new Fence(), Trans3d(3.50,-.055,-4.02,.08,.08,.08,0,-95,1.25));
	newEntity(new Fence(), Trans3d(3.36,-.055,-2.845,.08,.08,.08,0,-135,0));
	newEntity(new Fence(), Trans3d(2.51,-.045,-2.06,.08,.08,.08,0,-175,10));
	newEntity(new Fence(), Trans3d(1.47,.19,-1.95,.08,.08,.08,-7,-203,-9));
	newEntity(new Fence(), Trans3d(-2.78,1,-3.94,.08,.08,.08,0,-90,-6));
	newEntity(new Fence(), Trans3d(-2.78,.88,-2.86,.08,.08,.08,0,-80,-15.5));
	newEntity(new Fence(), Trans3d(-2.55,.57,-1.740,.08,.08,.08,8,0,1.9));
	newEntity(new Fence(), Trans3d(-1.461,.57,-1.733,.08,.08,.08,4,0,-3));
	newEntity(new Fence(), Trans3d(-.395,.52,-1.733,.08,.08,.08,0,5,-20));

	abductedCow.loadComponents();
	abductedCow.setTrans3d(Trans3d(2.2,-.3,-.2,.075,.075,.075,13,11,0));
	newEntity(new Cow(), Trans3d(-.395,.78,-3,.075,.075,.075,8,5,-5));
	newEntity(new Cow(), Trans3d(-.335,.793,-3.6,.04,.04,.04,8,10,-5));
	newEntity(new Cow(), Trans3d(-1.8,.70,-2.3,.075,.075,.075,8,-35,0));
	newEntity(new Cow(), Trans3d(2,.18,-5,.075,.075,.075,-14,-115,0));
	newEntity(new Cow(), Trans3d(2.3,.18,-3.2,.075,.075,.075,15,90,0));
	newEntity(new Cow(), Trans3d(.6,.58,-4.1,.075,.075,.075,15,115,0));
	newEntity(new Cow(), Trans3d(1.3,.2,-1.7,.075,.075,.075,13,45,0));
	//newEntity(new Cow(), Trans3d(2.2,-.3,-.2,.075,.075,.075,13,11,0));
	newEntity(new Cow(), Trans3d(1.2,-.290,.8,.075,.075,.075,-13,-170,9));
	newEntity(new Cow(), Trans3d(3.4,-.16,-1.5,.071,.071,.071,-20,150,10));

	newEntity(new Grass(), Trans3d(4.622539,-0.174740,-1.986022,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(6.062681,-0.092031,-3.991504,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-4.044989,1.036037,7.207368,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-5.288940,1.410261,-7.336454,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(1.023460,-0.800339,5.496870,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(1.323048,-0.897072,3.615448,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(3.862478,-1.231678,4.978537,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-6.128201,0.141514,0.894430,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-4.332521,0.477872,1.890721,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-7.348161,1.893830,6.342370,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-2.214361,0.116329,0.995587,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(3.188903,-0.535863,-0.060798,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(6.681075,-0.320906,0.398634,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(2.378507,0.162876,-6.200283,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-4.107805,0.937009,5.054699,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-3.038216,1.065222,-6.426837,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-1.212804,0.928409,-4.401934,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-4.205161,1.056785,-7.925969,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(7.586384,0.209521,-1.219835,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-3.470562,0.931057,6.937594,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(7.232677,0.143107,-4.361171,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-2.139071,0.331691,-0.845998,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(6.421673,-0.575139,5.705108,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(-7.176999,1.080124,-2.729841,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(5.917944,-0.034262,-5.769783,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(0.125830,-0.386250,7.174212,.08,.08,.08,0,0,0));
	newEntity(new Grass(), Trans3d(7.930378,-0.080872,7.669943,.08,.08,.08,0,0,0));

	newEntity(new Tree(), Trans3d(-5.940419,1.328664,-3.087941,.1,.1,.1,0,0,0));
	newEntity(new Tree(), Trans3d(-4.25291,0.336103,-0.21,.13,.13,.13,0,45,0));
	newEntity(new Tree(), Trans3d(.462,.54,-6.9,.08,.08,.08,0,100,0));

}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[]) {
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1000,800);
	glutCreateWindow("Jackson Markowski");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	LoadOBJ("../objects/barn.obj");

	generateEntities();

	dayTimeSky.setValues(.73, .913, .968, 1, .71,.913,.968,1.0, 50,75,65);
	currentTimeSky.setValues(.73, .913, .968, 1, .71,.913,.968,1.0, 50,75,65);
	nightTimeSky.setValues(.008,.008,.125,1, .008,.008,.125,1.0, 10,10,45);
	dayTimeSky.applyValues();

	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}