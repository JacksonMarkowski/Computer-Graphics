#include "CSCIx229.h"
#include "Ver3d.h"

//using namespace std;

class Camera {
	private:
		double camX = 3;
		double camY = 3;
		double camZ = 3;

		int th = -60;
		int ph = 30;

		double fov = 55; 
		double asp = 1.0;
		double dim = 3.0;

		int viewMode = PERSPECTIVE;
	public:
		enum CameraEnum {FIRST_PERSON, FOLLOW, PERSPECTIVE};
		Camera();
		void updateView(Ver3d followPos);
		void updateProjection();
		void setViewMode(CameraEnum viewMode);
		int getViewMode();
		void incTH(int value);
		void incPH(int value);
		void rotate(int th, int ph);
		void setCamPos(double camX, double camY, double camZ);
		//void incCamPos(double camXInc, double camYInc, double camZInc);
		void moveCamPosForward(double forwardInc);
		void moveCamPosSideways(double sidewaysInc);
		void setFOV(double fov);
		void setASP(double asp);
		void setDIM(double dim);
		void incDIM(double value);
};

Camera::Camera() {
	//loadComponents();
}

void Camera::updateView(Ver3d followPos) {
	switch(viewMode) {
		case FIRST_PERSON: {
			double centerX = camX + (-2*dim*Sin(th)*Cos(ph));
			double centerY = camY + (+2*dim        *Sin(ph));
			double centerZ = camZ + (+2*dim*Cos(th)*Cos(ph));
			gluLookAt(camX,camY,camZ , centerX,centerY,centerZ , 0,Cos(ph),0);
			break;
		}
		case FOLLOW: {
			int ph = -1*atan((followPos.y-camY)/(followPos.x-camX)) * 180 / 3.14;
			gluLookAt(camX,camY,camZ , followPos.x,followPos.y,followPos.z , 0,Cos(ph),0);
			break;
		}
		case PERSPECTIVE: {
			//Calculate postion of view based on viewing the origin
			double ex = -2*dim*Sin(th)*Cos(ph);
			double ey = +2*dim        *Sin(ph);
			double ez = +2*dim*Cos(th)*Cos(ph);
			gluLookAt(ex,ey,ez , 0,0,0 , 0,Cos(ph),0);
			break;
		}
	}
}

void Camera::updateProjection() {
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective transformation
	if (viewMode == FIRST_PERSON || viewMode == PERSPECTIVE || viewMode == FOLLOW)
		gluPerspective(fov,asp,dim/16,16*dim);
	//  Orthogonal projection
	else
		glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

void Camera::setViewMode(CameraEnum viewMode) {
	this->viewMode = viewMode;
}

int Camera::getViewMode() {
	return viewMode;
}

void Camera::incTH(int value) {
	this->th += value;
	this->th %= 360;
}

void Camera::incPH(int value) {
	this->ph += value;
	this->ph %= 360;
}

void Camera::rotate(int th, int ph) {
	this->th = th % 360;
	this->ph = ph % 360;
}

void Camera::setCamPos(double camX, double camY, double camZ) {
	this->camX = camX;
	this->camY = camY;
	this->camZ = camZ;
}
/*
void Camera::incCamPos(double camXInc, double camYInc, double camZInc) {
	this->camX += camXInc;
	this->camY += camYInc;
	this->camZ += camZInc
}*/

void Camera::moveCamPosForward(double forwardInc) {
	this->camX += -1*forwardInc*Sin(th)*Cos(ph);
	this->camY += +1*forwardInc        *Sin(ph);
	this->camZ += +1*forwardInc*Cos(th)*Cos(ph);
}

void Camera::moveCamPosSideways(double sidewaysInc) {
	this->camX += +1*sidewaysInc*Cos(th);
	this->camZ += +1*sidewaysInc*Sin(th);
}

void Camera::setFOV(double fov) {
	this->fov = fov;
}

void Camera::setASP(double asp) {
	this->asp = asp;
}

void Camera::setDIM(double dim) {
	this->dim = dim;
}

void Camera::incDIM(double value) {
	this->dim += value;
	if (this->dim < 1) this->dim = 1;
}