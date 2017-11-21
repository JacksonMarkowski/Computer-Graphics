#include "CSCIx229.h"
#include "Ver3d.h"

//using namespace std;

class Camera {
	private:
		double camX = -4;
		double camY = 3;
		double camZ = 4;

		int th = 25;
		int ph = 35;

		double fov = 55; 
		double asp = 1.0;
		double dim = 3.0;

		int viewMode = PERSPECTIVE;
	public:
		enum CameraEnum {FIRST_PERSON, ORTHOGONAL, PERSPECTIVE};
		Camera();
		void updateView();
		void updateProjection();
		void setViewMode(CameraEnum viewMode);
		void incTH(int value);
		void incPH(int value);
		void rotate(int th, int ph);
		void setFOV(double fov);
		void setASP(double asp);
		void setDIM(double dim);
		void incDIM(double value);
};

Camera::Camera() {
	//loadComponents();
}

void Camera::updateView() {
	switch(viewMode) {
		case FIRST_PERSON: {
			//Calculate where to look at based on position and view angle
			double centerX = camX + (-2*dim*Sin(th)*Cos(ph));
			double centerY = camY + (+2*dim        *Sin(ph));
			double centerZ = camZ + (+2*dim*Cos(th)*Cos(ph));
			gluLookAt(camX,camY,camZ , centerX,centerY,centerZ , 0,Cos(ph),0);
			break;
		}
		case ORTHOGONAL:
			//Standard orthogonal view
			glRotatef(ph,1,0,0);
			glRotatef(th,0,1,0);
			break;
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
	if (viewMode == FIRST_PERSON || viewMode == PERSPECTIVE)
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