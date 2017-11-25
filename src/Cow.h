#ifndef COW_H
#define COW_H

#include "Entity.h"

class Cow : public Entity {
	private:

	public:
		Cow();
		void draw();
		void loadComponents();
};

#endif