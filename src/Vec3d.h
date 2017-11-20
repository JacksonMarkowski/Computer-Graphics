#ifndef VEC3D
#define VEC3D

struct Vec3d {
	double x;
	double y;
	double z;
	Vec3d(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	Vec3d() {}
	void setValues(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
};

#endif