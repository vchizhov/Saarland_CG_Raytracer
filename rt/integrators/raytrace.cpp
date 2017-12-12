#include <rt/integrators/raytrace.h>

#include <rt/world.h>
#include <rt/ray.h>
#include <core/color.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/materials/material.h>
#include <rt/solids/solid.h>

namespace rt
{
	RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const
	{
		// final color
		RGBColor total = RGBColor::rep(0.0f);
		// intersect the scene
		Intersection result = world->scene->intersect(ray);
		if (result)
		{
			// iterate over the lights in the scene
			for (auto i = 0; i < world->light.size(); ++i)
			{
				// for each light: calculate the normalized vector and the distance from the hit point to the light
				// however we would like to offset the hit point a bit to avoid numerical precision errors like the hit point being underneath the surface of the object,
				// we use the normal as "exit" direction since it should be the fastest way "to exit" the object in the general case
				Point offsetHitPoint = result.hitPoint() + 100.0f*epsilon*result.normal();
				LightHit lightHit = world->light[i]->getLightHit(offsetHitPoint);
				// we only need to consider rays coming from the upper hemisphere around the normal
				// we know that dot(result.normal(), -ray.d)>=0.0f (we take care of that in the intersection ctor), then:
				if (0.0f<sgn(dot(result.normal(), lightHit.direction)))
				{
					// the shadow ray used to check whether the light is visible from the object
					Ray shadowRay(offsetHitPoint, lightHit.direction);
					// we could use a modified intersection routine, which exits early, rather than finding the closest intersection
					// it also does not need to calculate normals etc, since only whether there was an intersection matters
					Intersection inter = world->scene->intersect(shadowRay, lightHit.distance);
					// if there's no objects between the hit point and trhe light source
					if (!inter)
					{
						// calculate the brd weight based on the texture at the hit point, the normal at the hit point, and the incident and outgoing direction
						RGBColor brdfWeight = result.solid->material->getReflectance(result.local(), result.normal(), -ray.d, lightHit.direction);
						// same for the emission of the material, but we don't need to consider incoming radiance here
						RGBColor emission = result.solid->material->getEmission(result.local(), result.normal(), -ray.d);
						// add the contribution from the current light
						total = total + emission + brdfWeight*world->light[i]->getIntensity(lightHit);
					}
				}
			}
			
		}
		return total;
	}
}