#ifndef CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER
#define CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER

#include <rt/solids/solid.h>
#include <rt/ray.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
namespace rt {
	
class InfinitePlane : public Solid {
private:
	Point origin;
	Vector normal;
public:
    InfinitePlane() {}
	InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
		:origin(origin), normal(normal.normalize()), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		//// a tighter bound can be used for planes parallel to xy, xz, yz:
		//if (normal.x == 0.0f && normal.y == 0.0f) // parallel to xy
		//{
		//	return BBox(Point(-FLT_MAX, -FLT_MAX, origin.z-epsilon), Point( FLT_MAX, FLT_MAX, origin.z + epsilon));
		//}
		//else if (normal.x == 0.0f && normal.z == 0.0f) // parallel to xz
		//{
		//	return BBox(Point(-FLT_MAX, origin.y-epsilon, -FLT_MAX), Point(FLT_MAX, origin.y+epsilon, FLT_MAX));
		//}
		//else if (normal.y == 0.0f && normal.z == 0.0f) // parallel to yz
		//{
		//	return BBox(Point(origin.x - epsilon, -FLT_MAX, -FLT_MAX), Point(origin.x + epsilon, FLT_MAX, FLT_MAX));
		//}
		//else
			return BBox::full();
	}
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const
	{
		float dt = dot(ray.d, normal);
		// is the ray parallel to the plane - if it is just return no intersection
		// even if the ray lies exactly in the plane, I'd rather return no intersection
		//if (dt > -epsilon || dt < epsilon) return Intersection::failure();
		float t = dot(normal, origin - ray.o) / dt;
		// if the intersection is not in the ray's direction or there's a closer intersection - ignore it - no intersection
		if (t < 0.0f || t>previousBestDistance) return Intersection::failure();
		return Intersection(t, ray, this, normal, Point::rep(0.0f) + (ray.getPoint(t) - origin));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		// we need infinity
		return FLT_MAX;
	}
};

}


#endif
