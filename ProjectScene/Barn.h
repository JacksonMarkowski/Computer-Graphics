#ifndef BARN_H
#define BARN_H

#include "Entity.h"

class Barn : public Entity {
	public:
		Barn();
		~Barn();
		void draw();
		void loadComponents();
};

#endif