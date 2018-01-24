#ifndef VCDR_BACKGROUND_SKY_HEADER
#define VCDR_BACKGROUND_SKY_HEADER
#include <rendering_comp/vcdr_procedural.h>
#include <rendering_comp/vcdr_misc.h>
#include <rendering_comp/vcdr_solid.h>
namespace rt
{
	class VCDR_BackgroundSky : public VCDR_Solid
	{
	private:
		vec3 zenithColor;
		vec3 horizonColor;
		float height;
	public:
		VCDR_BackgroundSky(vec3 zenithColor = vec3(0.0f, 0.0f, 1.0f), vec3 horizonColor = vec3(1.0f), float height = 1000.0f)
			: zenithColor(zenithColor), horizonColor(horizonColor), height(height) {}

		VCDR_Intersection intersect(vec3 ro, vec3 rd, float tmin = 0.0f, float tmax = FLT_MAX) const
		{
			VCDR_Intersection res;
			res.obj = this;
			return res;
		}

		vec3 render(vec3 ro, vec3 rd, float t, const VCDR_World* world) const
		{
			// mix horizon and zenith color to get a nice gradient sky color
			vec3 col = mix(horizonColor, zenithColor, 0.5f*rd.y + 0.5f);

			// 2d fake clouds
			// height at which the clouds are
			float ti = (height - ro.y) / rd.y;
			if (ti>0.0f)
			{
				vec3 pos = ro + ti*rd;
				float density = 0.15f*0.6f*clamp(0.0f, 1.0f, FBM2D<>::fbm<true>(vec2(100.0f, 0.0f)+vec2(pos.x,pos.z)*0.002f*0.8f, 9, 0.55f, 1.9f));
				col = mix(col, 0.8f*vec3(1.0f), smoothstep(0.0f, 0.15f*0.6f*1.0f, density));
			}

			return col;
		}
	};
}

#endif
