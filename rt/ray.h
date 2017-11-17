#ifndef CG1RAYTRACER_RAY_HEADER
#define CG1RAYTRACER_RAY_HEADER

#include <core/vector.h>
#include <core/point.h>

namespace rt {

class Ray {
public:
    Point o;
    Vector d;

    Ray() {}
	Ray(const Point& o, const Vector& d) :o(o), d(d) {}
	Point getPoint(float distance) const
	{
		return o + distance*d;
	}
};

}

#endif
