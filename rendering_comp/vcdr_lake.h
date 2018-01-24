#ifndef VCDR_LAKE_HEADER
#define VCDR_LAKE_HEADER
#include <rendering_comp/vcdr_solid.h>
#include <core/random.h>
namespace rt
{
	class VCDR_Lake : public VCDR_Solid
	{
	private:
		vec3 lakeColor;
	private:
		float height;

		// From https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
		float rand(vec2 n) const
		{
			return fract(sin(dot(n, vec2(12.9898f, 4.1414f))) * 43758.5453f);
		}
	public:
		VCDR_Lake(float height, vec3 lakeColor)
			: height(height), lakeColor(lakeColor)
		{

		}

		// simple intersection with a plane: y=height
		VCDR_Intersection intersect(vec3 ro, vec3 rd, float tmin = 0.0f, float tmax = 30.0f) const
		{
			float t = (height - ro.y) / rd.y;
			if (t < 0.0f) return VCDR_Intersection();
			return VCDR_Intersection(t, this);
		}

		virtual vec3 render(vec3 ro, vec3 rd, float t, const VCDR_World* world) const
		{
			//vec3 col = vec3(0.3f, 0.48f, 1.0f);

			// initial color
			vec3 col;
			// position
			vec3 pos = ro + rd*t;
			// normal
			vec3 normal = vec3(0.0f, 1.0f, 0.0f);
			// perturb the normal
			normal = normalize(normal + vec3(0.0f, 0.0f, 0.08f*fabsf(sinf(7.0f*pos.z)) + 0.05f*(2.0f*FBM2D<>::fbm(vec2(pos.x, pos.z), 8, 0.5f, 2.0f) - 1.0f)));
			// reflected ray
			vec3 ref = reflect(rd, normal);
			// perturb the reflection ray
			//ref = normalize(ref + vec3(0.0f, 0.05f*sinf(3.0f*pos.z)+0.1f*(2.0f*FBM2D<>::fbm(vec2(pos.x, pos.z),8, 0.5f, 2.0f)-1.0f), 0.0f));

			// diffuse
			float cosTerm = max(0.0f, dot(world->info.sun.dir, normal));
			// specular
			vec3 spec = vec3(pow(max(0.0f, dot(ref, world->info.sun.dir)), 10.0f));
			
			//vec3 bon = normalize(ref - vec3(0.0f, 0.4f + 0.02f*(2.0f*rand(vec2(pos.x, pos.z)) - 1.0f), 0.0f));

			// transmitted ray
			vec3 dsp = pos - vec3(0.0f, 0.01f, 0.0f);
			VCDR_Intersection depth = world->intersect(dsp, rd);
			vec3 transmittedColor = vec3(0.0f);
			float dp = 1.0f;
			if (depth)
			{
				dp = clamp(0.0f, 1.0f, depth.t);
				transmittedColor = depth.obj->render(dsp, rd, depth.t, world);
			}
			// reflected ray
			vec3 dsp2 = pos + vec3(0.0f, 0.01f, 0.0f);
			VCDR_Intersection sky = world->intersect(dsp2, ref);
			vec3 reflectedColor = sky.obj->render(dsp2, ref, sky.t, world);

			// mix it up
			col = mix(transmittedColor, lakeColor, clamp(0.0f, 1.0f, 0.1f+dp));
			col = mix(reflectedColor, transmittedColor, 0.8f);

			col *= cosTerm*world->info.lightColor;
			col += spec;

			return vec3(col);
		}

	};

}

#endif
