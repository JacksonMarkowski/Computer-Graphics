#ifndef TREE_H
#define TREE_H

#include "Entity.h"

class Tree : public Entity {
	private:
		
	public:
		Tree();
		void draw();
		void loadComponents();
};

#endif