#ifndef TRANS3D
#define TRANS3D

#include "Ver3d.h"

struct Trans3d {
	Ver3d pos;
	Ver3d rot;
	Ver3d scale;
	Trans3d() {}
	Trans3d(double xPos, double yPos, double zPos, double xScale, double yScale, double zScale, double xRot, double yRot, double zRot) {
		pos.setValues(xPos, yPos, zPos); scale.setValues(xScale, yScale, zScale); rot.setValues(xRot, yRot, zRot);
	}
};

#endif