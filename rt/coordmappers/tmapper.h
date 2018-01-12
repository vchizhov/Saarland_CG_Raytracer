#ifndef CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER
#define CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER

#include <core/point.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class TriangleMapper : public CoordMapper {
private:
	Point tv0, tv1, tv2;
public:
	TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2) : tv0(tv0), tv1(tv1), tv2(tv2) {}
	TriangleMapper(Point ntv[3]) : tv0(ntv[0]), tv1(ntv[1]), tv2(ntv[2]) {}
	virtual Point getCoords(const Intersection& hit) const
	{
		Vector v0 = tv0 - Point::rep(0.0f);
		Vector v1 = tv1 - Point::rep(0.0f);
		Vector v2 = tv2 - Point::rep(0.0f);
		return Point::rep(0.0f) + hit.local().x*v0 + hit.local().y*v1 + hit.local().z*v2;
	}

};

}



#endif
