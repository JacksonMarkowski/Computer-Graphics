#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Entity.h"

struct light {
	double value;
	int brightening;
};

class Spaceship : public Entity {
	private:
		int metalSecTex, domeRingTex, domeTex;
		struct light lights[8];

		void drawWingPanel(Trans3d transform, int texture);
		void drawWingPanelSideEdge(Trans3d transform, int texture, double normal, double emission);
		void drawWingPanelInsideEdge(Trans3d transform, int texture);
		void drawWingPanelOutsideEdge(Trans3d transform, int texture);
		void drawWingBase(Trans3d transform, int texture, double upToj);
		void drawDomePanel(Trans3d transform, int texture);
		void drawDomeRing(Trans3d transform, int texture);
		void drawCenterRing(Trans3d transform, int texture);
		void drawBottomOpening(Trans3d transform, int texture);
		void drawBottomOpeningRing(Trans3d transform, int texture);

		void quadStripRotation(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY);
		void quadStripRotationTex(double startDegree, double endDegree, double incDegree, double r1, double r2, double y1, double y2, double normalY, double r1Tex, double r2Tex);
		
		void applyMetalMaterial();
		void applyLightMaterial();
	public:
		Spaceship();
		void update(double time);
		void draw();
		void loadComponents();
};

#endif