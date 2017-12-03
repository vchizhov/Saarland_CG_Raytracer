#ifndef CG1RAYTRACER_INTEGRATORS_MONTE_CARLO_HEADER
#define CG1RAYTRACER_INTEGRATORS_MONTE_CARLO_HEADER

#include <rt/integrators/integrator.h>

#include <rt/world.h>
#include <rt/ray.h>
#include <core/color.h>
#include <rt/intersection.h>
#include <random>
std::mt19937 randomEngine(1);
std::uniform_real_distribution<float> dist(0.0f, 1.0f);
namespace rt {

	class MonteCarloIntegrator : public Integrator {
	private:
		
	public:
		MonteCarloIntegrator(World* world) : Integrator(world){}
		virtual RGBColor getRadiance(const Ray& ray) const
		{
			uint32_t k = 0;
			float throughput = 1.0f;
			Intersection result;
			Ray temp = ray;
			do
			{
				result = world->scene->intersect(temp);
				if (!result)
					break;
				else
				{
					throughput *= max(0.0f, dot(-result.normal(), ray.d));
					//russian roullet
					//pick random point on hemisphere around the normal
					float phi = 2.0f*pi*dist(randomEngine);
					float theta = acosf(1.0f - 2.0f*dist(randomEngine));
					Vector rp = Vector(sinf(phi)*cosf(theta), sinf(phi)*sinf(theta), cosf(phi));
					if (dot(rp, result.normal()) < 0.0f) rp = -rp;
					Point randP = Point(rp.x, rp.y, rp.z);
					Point hitP = result.hitPoint();
					Vector d = (randP - hitP).normalize();
					temp = Ray(hitP + result.normal()*epsilon, d);
				}
				// 1) intersect
				//     a) hit nothing -> calculate dome lighting
				//     b) hit something -> attenuate throughput
				// 1.5) Russian roulette
				// 2) pick random point on hemishoere
				// 3) -> 1)
				++k;
			} while (k < 5 && throughput>0.0f);
			return throughput*RGBColor::rep(clamp(0.0f, 1.0f, 0.5f + 0.5f*temp.d.y));
		}
	};

}

#endif
