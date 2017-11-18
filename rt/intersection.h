#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Solid;

class Intersection {
public:

	//! the ray for which the intersection occurred
   Ray ray;

	//! the solid for which the intersection occured
    const Solid* solid;

	//! t for which the intersection occurred
    float distance;

	//! the normal at the intersection point
	Vector norm;

	//! intersection point
	Point hitP;

    Intersection() {}
	static Intersection failure()
	{
		Intersection fail;
		fail.distance = -1.0f;
		return fail;
	}
	Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv)
		:distance(distance), ray(ray), solid(solid), norm(normal)
	{

	}

	Point hitPoint() const
	{
		return ray.getPoint(distance);
	}
	Vector normal() const
	{
		return norm;
	}
	Point local() const
	{
		return ray.getPoint(distance);
	}

	operator bool() //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection
	{
		return distance >= 0.0f;
	}
};

}

#endif