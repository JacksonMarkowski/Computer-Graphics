#ifndef FENCE_H
#define FENCE_H

#include "Entity.h"

class Fence : public Entity {
	protected:
		static int fenceObj;
	public:
		Fence();
		~Fence();
		void draw();
		void loadComponents();
};

#endif