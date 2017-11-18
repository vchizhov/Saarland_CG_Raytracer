#ifndef CG1RAYTRACER_SOLIDS_SPHERE_HEADER
#define CG1RAYTRACER_SOLIDS_SPHERE_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>
#include <rt/bbox.h>

namespace rt {

class Sphere : public Solid {
private:
	Point center;
	float radius;
public:
    Sphere() {}
	Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
		: center(center), radius(radius), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		return BBox();
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		Vector oc = center - ray.o;
		float dt = dot(oc, ray.d);
		float discriminant = dt*dt + radius*radius - dot(oc, oc);
		// no intersection
		if (discriminant < 0.0f) return Intersection::failure();
		float dSqrt = sqrtf(discriminant);
		float t1 = dt - dSqrt;
		// previous intersection was closer
		if (t1>previousBestDistance) return Intersection::failure();
		// this is the closest intersection
		if (t1 > 0.0f)
		{
			Vector diff = (ray.getPoint(t1) - center)/radius;
			return Intersection(t1, ray, this, diff, Point(acosf(diff.z), atanf(diff.y / diff.x), 0.0f));
		}
		float t2 = dt + dSqrt;
		// either the intersection was farther than the closest one, or it was behind us
		if (t2 < 0.0f || t2>previousBestDistance) return Intersection::failure();
		//this is the closest intersection
		Vector diff = (ray.getPoint(t2) - center)/radius;
		return Intersection(t2, ray, this, diff, Point(acosf(diff.z), atanf(diff.y/diff.x), 0.0f));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		return 4.0f*pi*radius*radius;
	}
};

}


#endif
