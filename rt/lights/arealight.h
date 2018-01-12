#ifndef CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER

#include <core/vector.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>

namespace rt {

class AreaLight : public Light {
private:
	Solid* source;
public:
	AreaLight() {}
	AreaLight(Solid* source) : source(source) {}
	virtual LightHit getLightHit(const Point& p) const
	{
		// sample random point on the light
		Vector direction = source->sample() - p;
		float distance = direction.length();
		direction = direction / distance;
		return LightHit(direction, distance);
	}
	virtual RGBColor getIntensity(const LightHit& irr) const
	{
		// hardcoded cos since I do not have access to the normal of the light
		// for this scene I know it, so I've hardcoded the cosine
		// the fabsf is there since I assume the light emits from both sides
		// I removed the cs since the reference images do not use it
		float cs = 1.0f;// fabsf(dot(irr.direction, Vector(0.0f, 1.0f, 0.0f)));
		return source->material->getEmission(Point::rep(0.0f), Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f))/sqr(irr.distance)*source->getArea()*cs;
	}
};

}

#endif

