#ifndef FENCE_H
#define FENCE_H

#include "Entity.h"

class Fence : public Entity {
	public:
		Fence();
		void draw();
		void loadComponents();
};

#endif