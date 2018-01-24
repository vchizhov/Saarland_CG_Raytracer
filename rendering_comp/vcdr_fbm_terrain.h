#ifndef VCDR_FBM_TERRAIN_HEADER
#define VCDR_FBM_TERRAIN_HEADER

#include <rendering_comp/vcdr_solid.h>
#include <rendering_comp/vcdr_procedural.h>

namespace rt
{
	class VCDR_FBMTerrain : public VCDR_Solid
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
		// terrain height
		float height;
		// terrain scale
		float scale;

		// from iquilez's article on fake soft shadows: http://iquilezles.org/www/articles/rmshadows/rmshadows.htm
		float shadow(vec3 ro, vec3 rd) const
		{
			const float tmin = 0.01f;
			float t = tmin;
			float acc = 1.0f;
			for (int i = 0; i<32; i++)
			{
				vec3  pos = ro + t*rd;
				float  terrainHeight = map(pos);
				float hei = pos.y - terrainHeight;
				acc = min(acc, 32.0f*hei / t);
				if (acc<0.0001f) break;
				t = clamp(1.0f + t*0.1f, 50.0f, hei);
			}

			return clamp(0.0f, 1.0f, acc);
		}

	public:
		VCDR_FBMTerrain(float scale, float height, int layers = 4, float AM = 0.5f, float FM = 2.0f, float deltaScaling = 0.1f, float startingDelta = 0.1f)
			: layers(layers), AM(AM), FM(FM), deltaScaling(deltaScaling), startingDelta(startingDelta), scale(scale), height(height)
		{}

		float map(vec3 v) const
		{
			return height*FBM2D<>::fbm(scale*vec2(v.x, v.z), layers, AM, FM);
		}

		vec3 calculateNormal(vec3 v) const
		{
			vec3 res = scale*height*FBM2D<>::fbmD(scale*vec2(v.x, v.z), layers, AM, FM);
			return normalize(vec3(-res.y, 1.0f, -res.z));
		}

		 VCDR_Intersection intersect(vec3 ro, vec3 rd, float tmin, float tmax) const
		{

			// used for linear interpolation (better intersection):
			// last ray y
			float ly = 0.0f;
			// last terrain y
			float lh = 0.0f;

			// integrate over the ray path with step size delta
			float delta = startingDelta;

			for (float t = tmin; t<tmax; t += delta)
			{
				// current sample point on ray
				vec3 p = ro + t*rd;
				// evaluate terrain height
				float h = map(p);
				// if the ray point is beneath the terrain - find the intersection
				if (p.y<h)
				{
					// linearly interpolate the last point and the current one, to estimate
					// the ray-terrain intersection (improves intersection quality quite a bit)
					float resT = t - delta + delta*(lh - ly) / (p.y - ly - h + lh);
					vec3 pos = ro + resT*rd;
					return VCDR_Intersection(resT, this);
				}
				// record the last ray sample y and terrain y
				ly = p.y;
				lh = h;
				// rescale delta for a speedup
				delta = startingDelta + t*deltaScaling;
			}
			return VCDR_Intersection();
		}

		vec3 render(vec3 ro, vec3 rd, float t, const VCDR_World* world) const
		{
			vec3 col = vec3(0.0f);
			// find intersection between the ray and the terrain
			// VCDR_Intersection res = intersect(ro, rd, 0.0f, 20.0f);

			// intersection position
			vec3 pos = ro + rd*t;
			// initial normal
			vec3 norm = calculateNormal(pos);
			// normal perturbation:
			// the bigger the slope is - the more perturbed the normal is
			float perturbWeight = 0.32f*(1.0f - fabsf(norm.y));
			// frequency of details along y should be lower
			vec3 perturbFrequency = 0.3f*vec3(1.0f, 0.1f, 1.0f);
			// random perturbation
			vec3 perturbNormal = FBM3D<>::fbmD<true>(perturbFrequency*pos, 7, 0.5f, 1.95f).yzw();
			vec3 normal = normalize(norm + perturbWeight*perturbNormal);


			// terrain colours
			// non-steep slopes colour:
			const vec3 green = vec3(0.3f, 0.3f, 0.0f);
			// steep slopes colour:
			const vec3 slope = vec3(0.18f, 0.11f, 0.10f)*0.75f;

			// mix based on slope (normal.y)
			vec3 interm = mix(slope, green, smoothstep(0.7f, 0.9f, normal.y));
			// fake ambient occlusion
			float occ = pos.y / (1.5f*height);

			// diffuse lighting
			float cosTerm = max(0.0f, dot(world->info.sun.dir, normal));
			float shad = 1.0f;
			//if (cosTerm>0.001f) shad = clamp(0.1f, 1.0f, shadow(pos + normal*0.01f, world->info.sun.dir));

			// modulate color
			vec3 lighting = world->info.lightColor*cosTerm;
			col = interm*world->info.lightColor*cosTerm*occ*shad;
			return col;
		}

	};
}

#endif
