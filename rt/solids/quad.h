#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>

namespace rt {

class Quad : public Solid {
private:
	Point v1;
	Vector span1;
	Vector span2;
public:
    Quad() {}
	Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
		: v1(v1), span1(span1), span2(span2), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		Point min = rt::min(v1, v1 + span1, v1 + span2, v1 + span1 + span2);
		Point max = rt::min(v1, v1 + span1, v1 + span2, v1 + span1 + span2);
		return BBox(min, max);
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		// v1 + beta*span1 + gamma*span2 = p, p is inside the quad for 0<=beta<=1 and 0<=gamma<=1
		// plugging in p = ray.o+ray.d*t =>
		// beta*span1 + gamma*span2 - ray.d*t = ray.o - v1, we have the same situation as for the triangle
		Vector ab = cross(span1, span2);
		float det = -dot(ab, ray.d);
		// det==0 -> we either have a parallel ray that does not intersect the triangle, or is inside the triangle's plane
		//if (-epsilon < det && det < epsilon) return Intersection::failure();
		float invDet = 1.0f / det;
		// Cramer's rule (probably not the most effective method, but it looks more elegant)
		Vector c = ray.o - v1;
		float detT = dot(ab, c);
		float t = detT * invDet;
		// if the intersection is behind or is farther than the closest one - ignore it - no intersection
		if (t<0.0f || t>previousBestDistance) return Intersection::failure();
		float beta = dot(c, cross(ray.d, span2)) * invDet;
		float gamma = dot(span1, cross(ray.d, c)) * invDet;
		// if the intersection is outside of the quad - ignore it - no intersection
		if (beta < 0.0f || beta > 1.0f || gamma<0.0f || gamma>1.0f) return Intersection::failure();
		return Intersection(t, ray, this, ab.normalize(), Point(beta, gamma, 0.0f));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		return cross(span1, span2).length();
	}
};

}

#endif
