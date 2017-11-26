#ifndef ENTITY_H
#define ENTITY_H

#include "CSCIx229.h"
#include "Ver3d.h"
#include "Trans3d.h"

class Entity {
	protected:
		int obj;
		int tex;
		Trans3d transformation;
		void applyTrans3d(Trans3d transform);
	public:
		Entity();
		virtual void draw();
		virtual void loadComponents();
		void update(double elapsedTime);
		void setTrans3d(Trans3d transformation);
		double incX(double x);
		double incY(double y);
		double incZ(double z);
		Ver3d getPos();
};

#endif