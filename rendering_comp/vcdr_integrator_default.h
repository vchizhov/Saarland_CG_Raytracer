#ifndef VCDR_INTEGRATOR_DEFAULT_HEADER
#define VCDR_INTEGRATOR_DEFAULT_HEADER
#include <rendering_comp/vcdr_integrator.h>
#include <rendering_comp/vcdr_misc.h>
#include <rendering_comp/vcdr_cloud.h>
namespace rt
{

	

	class VCDR_IntegratorDefault : public VCDR_Integrator
	{
	private:
		VCDR_Cloud* cloud;
	public:
		VCDR_IntegratorDefault(VCDR_Cloud* cloud) : cloud(cloud) {}

		vec3 getColor(vec3 ro, vec3 rd, const VCDR_World* world) const
		{
			

			VCDR_Intersection result = world->intersect(ro, rd);
			vec3 col;
			float sunStrength = clamp(0.0f, 1.0f, dot(world->info.sun.dir, rd));
			col = result.obj->render(ro, rd, result.t, world);
			
			if (!result)
			{
				// fix fog for the sky
				float ti = (world->info.skyHeight - ro.y) / rd.y;
				if (ti > 0.0f) result.t = ti;
				col = mix(col, world->info.fogColor, clamp(0.0f, 1.0f, result.t / (15.0f*world->info.skyHeight)));
				col += world->info.sun.col*powf(sunStrength, world->info.sun.size);
			}
			else
			{
				col = fog(col, world->info.fogColor, result.t, world->info.fogAttenuation);
			}
			

			// volumetric clouds
			VCDR_CloudInter ci = cloud->intersect(ro, rd, result.t);
			if (ci)
			{
				col = cloud->render(ro, rd, ci.t1, ci.t2, col);
			}

			// fake sun glare
			col += world->info.sun.glareCol*powf(sunStrength, world->info.sun.glareSize);
			
			

			
			//col = sqrt(col);
			return col;
		}
	};
}

#endif
