#ifndef CG1RAYTRACER_LIGHTS_SPOTLIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_SPOTLIGHT_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/lights/light.h>

namespace rt {

class SpotLight : public Light {
private:
	Point position;
	Vector direction;
	float cosAngle;
	float exp;
	RGBColor intensity;
public:
	SpotLight() {}
	SpotLight(const Point& position, const Vector& direction, float angle, float exp, const RGBColor& intensity)
		: position(position), direction(direction.normalize()), cosAngle(cosf(angle)), exp(exp), intensity(intensity) {}
	virtual LightHit getLightHit(const Point& p) const
	{
		Vector direction = position - p;
		float distance = direction.length();
		direction = direction / distance;
		return LightHit(direction, distance);
	}
	virtual RGBColor getIntensity(const LightHit& irr) const
	{
		float cosA = dot(-irr.direction, direction);
		if (cosA <= cosAngle)
			return RGBColor::rep(0.0f);
		else
		{
			float distSq = irr.distance*irr.distance;
			return intensity*powf(cosA, exp) / distSq;
		}
	}
};

}

#endif

