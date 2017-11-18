#ifndef CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER
#define CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER

#include <rt/integrators/integrator.h>

#include <rt/world.h>
#include <rt/ray.h>
#include <core/color.h>
#include <rt/intersection.h>

namespace rt {


class RayCastingDistIntegrator : public Integrator {
public:
	RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
		: Integrator(world), nearColor(nearColor), nearDist(nearDist), farColor(farColor), farDist(farDist)
	{

	}
    virtual RGBColor getRadiance(const Ray& ray) const
	{
		Intersection result = world->scene->intersect(ray);
		if (result)
		{
			float dt = fabsf(dot(-result.normal(), ray.d));
			float interp = (rt::clamp(nearDist, farDist, result.distance) - nearDist) / (farDist - nearDist);
			return (dt*((1.0f - interp)*nearColor + interp*farColor)).clamp();
		}
		else
			return RGBColor::rep(0.0f);
	}
private:
    RGBColor nearColor, farColor;
    float nearDist, farDist;
};

}

#endif
