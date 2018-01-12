#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>
#include <rt/bbox.h>
#include <core/random.h>

namespace rt {

class Triangle : public Solid {
public:
	Point v1, v2, v3;
public:
    Triangle() {}
	Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
		: v1(vertices[0]), v2(vertices[1]), v3(vertices[2]), Solid(texMapper, material)
	{

	}
	Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
		: v1(v1), v2(v2), v3(v3), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		Point min = rt::min(v1, v2, v3);
		Point max = rt::max(v1, v2, v3);
		return BBox(min, max);
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		// alpha*v1+beta*v2+gamma*v3 = p, and p is inside tri(v1,v2,v3) iff alpha,beta,gamma>=0 and alpha+beta+gamma=1, then
		// (1-beta-gamma)*v1+beta*v2+gamma*v3 = p =>
		// plug in p = ray.o + ray.d*t =>
		// (v2-v1)*beta + (v3-v1)*gamma - ray.d*t = ray.o - v1 => 3 equations (for x,y,z), 3 unknowns (beta, gamma, t):
		Vector a = v2 - v1;
		Vector b = v3 - v1;
		Vector ab = cross(a, b);
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
		float beta = dot(c, cross(ray.d, b)) * invDet;
		float gamma = dot(a, cross(ray.d, c)) * invDet;
		// if the intersection is outside of the triangle - ignore it - no intersection
		if (beta < 0.0f || gamma<0.0f || beta + gamma>1.0f) return Intersection::failure();
		return Intersection(t, ray, this, ab.normalize(), Point(1.0f - (beta + gamma), beta, gamma));
	}
	virtual Point sample() const
	{
		float sr1 = sqrtf(random());
		float r2 = random();
		float alpha = 1.0f - sr1;
		float beta = r2*sr1;
		return v2 + (v1 - v2) * alpha + (v3 - v2) * beta;
	}
	virtual float getArea() const
	{
		return 0.5f*cross(v2 - v1, v3 - v1).length();
	}
};

}

#endif
