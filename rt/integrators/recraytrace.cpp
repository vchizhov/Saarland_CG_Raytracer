#include <rt/integrators/recraytrace.h>

#include <rt/world.h>
#include <rt/ray.h>
#include <core/color.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/materials/material.h>
#include <rt/solids/solid.h>

namespace rt
{
	RGBColor RecursiveRayTracingIntegrator::directIllumination(const Intersection& result) const
	{
		RGBColor total = RGBColor::rep(0.0f);
		Point texPoint = result.solid->texMapper->getCoords(result);
		RGBColor emission = result.solid->material->getEmission(texPoint, result.normal(), -result.ray.d);
		total = total + emission;
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
			if (0.0f < sgn(dot(result.normal(), lightHit.direction)))
			{
				// the shadow ray used to check whether the light is visible from the object
				Ray shadowRay(offsetHitPoint, lightHit.direction);
				// we could use a modified intersection routine, which exits early, rather than finding the closest intersection
				// it also does not need to calculate normals etc, since only whether there was an intersection matters
				Intersection inter = world->scene->intersect(shadowRay, lightHit.distance - 100.0f*epsilon);
				// if there's no objects between the hit point and trhe light source
				if (!inter)
				{
					// calculate the brd weight based on the texture at the hit point, the normal at the hit point, and the incident and outgoing direction
					RGBColor brdfWeight = result.solid->material->getReflectance(texPoint, result.normal(), -result.ray.d, lightHit.direction);

					// add the contribution from the current light
					total = total + brdfWeight*world->light[i]->getIntensity(lightHit);
				}
			}
		}
		return total;
	}

	RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const
	{
		// final color
		RGBColor total = RGBColor::rep(0.0f);
		// radiance weight
		RGBColor throughput = RGBColor::rep(1.0f);
		// the current ray
		Ray currentRay = ray;
		
		// intersect the scene
		Intersection result = world->scene->intersect(currentRay);

		// permit no more than 6 light bounces
		// russian roulette may be used, but we still need a hard ceiling for cases where we have parallel mirrors
		// with perfect reflectance
		for(int iter=0; iter < 6; ++iter)
		{

			// no intersection  or no sampling required -> exit out of the loop
			if (!result || result.solid->material->useSampling() == Material::SAMPLING_NOT_NEEDED) break;

			// if we also need to sample the lights -> do it
			if (result.solid->material->useSampling() == Material::SAMPLING_SECONDARY)
				total = total + throughput*directIllumination(result);

			// get the sampled direction and the corresponding reflectance
			// we have only perfect reflection for now, so we know that the resulting direction
			// is in the same hemisphere as the normal
			Point texPoint = result.solid->texMapper->getCoords(result);
			Material::SampleReflectance refl = result.solid->material->getSampleReflectance(texPoint,
				result.normal(), -currentRay.d);

			// take into account the percent of reflected energy 
			throughput = throughput*refl.reflectance;

			// prepare the next ray
			float sgnMod = 1.0f;
			if (dot(refl.direction, result.normal()) < 0.0f)
				sgnMod = -1.0f;
			currentRay = Ray(result.hitPoint() + sgnMod*100.0f*epsilon*result.normal(), refl.direction); // Ray(result.hitPoint() + 100.0f*epsilon*result.normal(), refl.direction);

			// find the next intersection
			result = world->scene->intersect(currentRay);
		}

		if (result)
			total = total + throughput*directIllumination(result);

		return total;
	}
}