#ifndef CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Intersection;
class SphericalCoordMapper : public CoordMapper {
private:
	Point origin;
	Vector zenith;
	Vector azimuth;
	float zMult;
	float aMult;
public:
	SphericalCoordMapper() {}
	SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
	: origin(origin), zMult(zenith.length()), aMult(azimuthRef.length())
	{
		Vector temp = cross(zenith, azimuthRef);
		this->azimuth = cross(temp, zenith).normalize();
		this->zenith = zenith / zMult;
	}
	virtual Point getCoords(const Intersection& hit) const
	{
		Vector tr = (hit.local() - origin).normalize();
		Vector temp = cross(zenith, azimuth);
		tr = Vector(dot(tr, azimuth), dot(tr, temp), dot(tr, zenith)).normalize();
		float v = acosf(tr.z) / zMult / pi;
		float u = 0.5f * atan2f(tr.y, tr.x) / aMult / pi;
		return Point(u, v, 0.0f);
	}
};

}

#endif
