#ifndef TERRAIN_H
#define TERRAIN_H

#include "Entity.h"

class Terrain : public Entity {
	public:
		Terrain();
		void draw();
		void loadComponents();
};

#endif