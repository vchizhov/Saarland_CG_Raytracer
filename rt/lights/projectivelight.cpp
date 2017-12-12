#include <rt/lights/projectivelight.h>
#include <rt/ray.h>

float a2computeWeight(float fx, float fy, const rt::Point& c, float div);
rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt
{
	RGBColor ProjectiveLight::getIntensity(const LightHit& irr) const
	{
		float distSq = irr.distance*irr.distance;
		// intensity diminishes with r^2
		// we have a minus since irr.direction starts at the hit point and ends at the light,
		// and a2computeColor should take a ray coming out of the light
		return intensity*a2computeColor(Ray(position, -irr.direction)) / distSq;
	}
}