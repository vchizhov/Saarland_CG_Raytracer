#ifndef CG1RAYTRACER_LIGHTS_POINTLIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_POINTLIGHT_HEADER

#include <core/point.h>
#include <core/color.h>
#include <rt/lights/light.h>

namespace rt {

class PointLight : public Light {
private:
	Point position;
	RGBColor intensity;
public:
	PointLight() {}
	PointLight(const Point& position, const RGBColor& intensity) :
		position(position), intensity(intensity)
	{

	}
	virtual LightHit getLightHit(const Point& p) const
	{
		Vector direction = position - p;
		float distance = direction.length();
		// normalize
		direction = direction / distance;
		return LightHit(direction, distance);
	}
	virtual RGBColor getIntensity(const LightHit& irr) const
	{
		float distSq = irr.distance*irr.distance;
		// intensity diminishes with r^2
		return intensity / distSq;
	}
};

}

#endif

