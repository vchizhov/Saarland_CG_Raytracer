#ifndef CG1RAYTRACER_INTEGRATORS_CASTING_HEADER
#define CG1RAYTRACER_INTEGRATORS_CASTING_HEADER

#include <rt/integrators/integrator.h>

#include <rt/world.h>
#include <rt/ray.h>
#include <core/color.h>
#include <rt/intersection.h>

namespace rt {

class RayCastingIntegrator : public Integrator {
public:
    RayCastingIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const
	{
		Intersection result = world->scene->intersect(ray);
		if (result)
		{
			float dt = fabsf(dot(-result.normal(), ray.d));
			return RGBColor::rep(dt);
		}
		else
			return RGBColor::rep(0.0f);
	}
};

}

#endif
