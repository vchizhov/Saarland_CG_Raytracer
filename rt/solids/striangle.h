#ifndef CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <rt/solids/triangle.h>
#include <rt/intersection.h>
#include <core/point.h>

namespace rt {

class SmoothTriangle : public Triangle {
private:
	Vector n1, n2, n3;
public:
    SmoothTriangle() {}
	SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
		:Triangle(vertices, texMapper, material), n1(normals[0]), n2(normals[1]), n3(normals[2]) {}
	SmoothTriangle(const Point& v1, const Point& v2, const Point& v3,
		const Vector& n1, const Vector& n2, const Vector& n3,
		CoordMapper* texMapper, Material* material)
		: Triangle(v1, v2, v3, texMapper, material), n1(n1), n2(n2), n3(n3) {}

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
		return Intersection(t, ray, this, ((1.0f - (beta + gamma))*n1 + beta*n2 + gamma*n3).normalize(), Point(1.0f - (beta + gamma), beta, gamma));
	}
};

}

#endif
