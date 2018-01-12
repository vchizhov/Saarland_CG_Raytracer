#ifndef CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER
#define CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Intersection;

class PlaneCoordMapper : public CoordMapper{
private:
	Vector e1, e2;
public:
	PlaneCoordMapper(const Vector& e1, const Vector& e2) : e1(e1), e2(e2) {}
	virtual Point getCoords(const Intersection& hit) const
	{
		// this is what I got from the way the assignemnet was defined:
		// this is a projection of the local point onto the plane
		// with origin in (0,0,0) and 2 vectors e1,e2
		// the length is squared to take into account the scale of e1 and e2
		float u = dot(hit.local() - Point::rep(0.0f), e1)/e1.lensqr();
		float v = dot(hit.local() - Point::rep(0.0f), e2)/e2.lensqr();
		// maybe r = e1*u + e2*v was required?
		return Point(u, v, 0.0f);
	}
};

}

#endif
