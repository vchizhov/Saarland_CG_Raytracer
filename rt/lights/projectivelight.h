#ifndef CG1RAYTRACER_LIGHTS_PROJECTIVELIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_PROJECTIVELIGHT_HEADER

#include <core/point.h>
#include <core/color.h>
#include <rt/lights/light.h>

namespace rt {

	class ProjectiveLight : public Light {
	private:
		Point position;
		RGBColor intensity;
	public:
		ProjectiveLight() {}
		ProjectiveLight(const Point& position, const RGBColor& intensity) :
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
		virtual RGBColor getIntensity(const LightHit& irr) const;
	};

}

#endif
