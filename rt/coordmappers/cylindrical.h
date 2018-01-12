#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>
namespace rt {

class Intersection;

class CylindricalCoordMapper : public CoordMapper {
private:
	Point origin;
	Vector longitudinalAxis, polarAxis;
public:
	CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
		: origin(origin), longitudinalAxis(longitudinalAxis)
	{
		Vector temp = cross(longitudinalAxis, polarAxis);
		Vector prp = cross(temp, longitudinalAxis);
		prp.normalize();
		this->polarAxis = prp * polarAxis.length();
	}
	virtual Point getCoords(const Intersection& hit) const
	{
		// let p be the local point, o be the origin, a be the logitudinal axis
		// then for a projection q of p on the longitudinal axis holds:
		// dot(p-q, o-q) = 0  ->  dot(p - o - a*t, o - o - a*t) = 0 -> t*(dot(a,a)*t + dot(a, o-p)) = 0 -> t = dot(a,p-o)/dot(a,a)
		float v = dot(hit.local() - origin, longitudinalAxis) / dot(longitudinalAxis, longitudinalAxis) / longitudinalAxis.length();
		// for u we need to find the angle between the polarAxis and p-q
		Vector qp = (hit.local() - origin - v*longitudinalAxis).normalize();
		float u = acosf(dot(qp, polarAxis.normalize()));
		// check whether + pi
		if (dot(cross(polarAxis, qp), longitudinalAxis) > 0.0f)
			u += pi;
		// map to [0,1]
		u = 0.5f*u / pi / polarAxis.length();
		return Point(u, v, 0);
	}
};

}

#endif
