#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>

namespace rt {

class Disc : public Solid {
private:
	Point center;
	float radius;
	Vector normal;
public:
    Disc() {}
	Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
		: center(center), normal(normal.normalize()), radius(radius), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		return BBox();
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		float dt = dot(ray.d, normal);
		// is the ray parallel to the plane - if it is just return no intersection
		// even if the ray lies exactly in the plane, I'd rather return no intersection
		//if (dt > -epsilon || dt < epsilon) return Intersection::failure();
		float t = dot(normal, center - ray.o) / dt;
		// if the intersection is not in the ray's direction or there's a closer intersection - ignore it - no intersection
		if (t < 0.0f || t>previousBestDistance) return Intersection::failure();
		// if the intersection is outside of the disk - ignore it - no intersection
		Vector dst = ray.getPoint(t) - center;
		if (dot(dst, dst) > radius*radius) return Intersection::failure();
		return Intersection(t, ray, this, normal, Point::rep(0.0f));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		return pi*radius*radius;
	}
};

}


#endif
