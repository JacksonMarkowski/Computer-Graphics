#ifndef SMOKECLOUD_H
#define SMOKECLOUD_H

#include "Entity.h"

struct cloudPiece {
	double r;
	double y;
	double x;
};

class SmokeCloud : public Entity {
	private:
		struct cloudPiece pieces[5];
		void drawSmokePiece(double x, double y, double z, double r);
		void Vertex(double th,double ph);
	public:
		SmokeCloud();
		~SmokeCloud();
		void draw();
		void loadComponents();
		int update(double time);
};

#endif