#ifndef VER3D
#define VER3D

struct Ver3d {
	double x;
	double y;
	double z;
	Ver3d(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	Ver3d() {}
	void setValues(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
};

#endif