#ifndef CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER
#define CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/intersection.h>
#include <core/float4.h>
#include <core/point.h>

namespace rt {

class WorldMapper : public CoordMapper {
private:
	Float4 scale;
public:
	WorldMapper() : scale(Float4::rep(1.0f)) {}
	explicit WorldMapper(const Float4& scale) : scale(scale) {}
	virtual Point getCoords(const Intersection& hit) const
	{
		const Point p = hit.hitPoint();
		return Point(p.x*scale.x, p.y*scale.y, p.z*scale.z);
	}
};

}

#endif
