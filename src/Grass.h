#ifndef GRASS_H
#define GRASS_H

#include "Entity.h"

class Grass : public Entity {
	private:
		
	public:
		Grass();
		~Grass();
		void draw();
		void loadComponents();
};

#endif