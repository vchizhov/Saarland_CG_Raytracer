#ifndef VCDR_CLOUDS_HEADER
#define VCDR_CLOUDS_HEADER

#include <rendering_comp/vcdr_solid.h>
#include <rendering_comp/vcdr_procedural.h>

namespace rt
{
	class VCDR_Clouds : public VCDR_Solid
	{
	private:
		// fbm layers
		int layers;
		// fbm amplitude and frequency multiplication factors
		float AM, FM;
		// linear step size scaling with respect to t
		float deltaScaling;
		// starting step size
		float startingDelta;
		// clouds density
		float densityMultiplier;
		// clouds scale
		vec3 scale;
	public:
		VCDR_Clouds(vec3 scale, float densityMultiplier, int layers = 4, float AM = 0.5f, float FM = 2.0f, float deltaScaling = 0.1f, float startingDelta = 0.1f)
			: layers(layers), AM(AM), FM(FM), deltaScaling(deltaScaling), startingDelta(startingDelta), scale(scale), densityMultiplier(densityMultiplier)
		{}

		float map(vec3 v) const
		{
			//float hx = smoothstep(-10.0f, 0.0f, v.x) - smoothstep(0.0f, 10.0f, v.x);//smoothstep(-10.0f, 0.0f, v.x);//
			float hx = 1.0f;
			float hy = smoothstep(-10.0f, 0.0f, v.y) - smoothstep(0.0f, 10.0f, v.y);
			float n = densityMultiplier*FBM3D<>::fbmc<true>(scale*(v + vec3(8.0f,0.0f,3.0f)), layers, AM, FM);
			return n*hx;
		}

		float integrateDensity(vec3 ro, vec3 rd) const
		{
			float tmin = 0.0f;
			float tmax = 10.0f;
			float delta = startingDelta;
			float integratedDensity = 0.0f;
			float t = tmin;
			for (; t<tmax; t += delta)
			{
				vec3 pos = ro + t*rd;

				float d = max(0.0f, map(pos));
				integratedDensity += d*delta;
				if (integratedDensity>1.0f) break;
				//delta = startingDelta;// +t*deltaScaling;
			}
			return min(integratedDensity, 1.0f);
		}

		VCDR_Intersection intersect(vec3 ro, vec3 rd, float tmin, float tmax) const
		{
			const float ox = 30.0f;
			const float oy = -5.0f;
			float t = (20.0f - ro.z) / rd.z;
			vec3 pos = ro + rd*t;
			if (t > 0.0f && pos.x-ox>-10.0f/2.0f && pos.x-ox<10.0f/2.0f && pos.y-oy<10.0f/2.0f && pos.y-oy>-10.0f/2.0f) return VCDR_Intersection(t, this);
			return VCDR_Intersection();
		}

		vec3 render(vec3 ro, vec3 rd, float t, const VCDR_World* world) const
		{
			const vec3 sky = vec3(0.4f, 0.6f, 1.0f);
			vec3 att = vec3(0.2f, 0.5f, 0.9f);
			float density = integrateDensity(ro+t*rd, rd);
			//float shad = 0.9f*(1.0f - shadeClouds(pos + sunDir*0.1f, sunDir));
			float inv = (1.0f - density);

			float w = 3.0f*(0.5f*rd.y + 0.5f);
			vec3 cl = mix(vec3(0.0f), vec3(1.0f), density);
			//if (density<0.1f) return mix(sky, cl, max(0.0f, density)*10.0f);
			//vec3 col = mix(sky, cl, 1.0f-exp(-0.0003*ns.y*ns.y) );
			//if(ns.y<0.0f) return sky;
			return cl;
		}
	};
}

#endif
