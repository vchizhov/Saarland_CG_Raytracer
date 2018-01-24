#ifndef VCDR_CLOUD_HEADER
#define VCDR_CLOUD_HEADER
#include <rendering_comp/vcdr_vec.h>
namespace rt
{
	struct VCDR_CloudInter
	{
		float t1, t2;
		bool inter;

		VCDR_CloudInter() : inter(false) {}
		VCDR_CloudInter(float t1, float t2) : inter(true), t1(t1), t2(t2) {}

		operator bool() const
		{
			return  inter;
		}
	};
	class VCDR_Cloud
	{
	private:
		vec3 k;
		vec3 c;
	public:
		VCDR_Cloud(vec3 center, vec3 scale) : c(center), k(scale) {}
		VCDR_CloudInter intersect(vec3 ro, vec3 rd, float tmax)
		{
			vec3 dk = rd / k;
			vec3 ock = (c - ro) / k;
			float A = dk.lengthsqr();
			float B = dot(ock, dk);
			float C = ock.lengthsqr() - 1.0f;
			float det = B*B - A*C;
			if (det < 0.0f) return VCDR_CloudInter();
			det = sqrtf(det);
			float t1 = (B - det) / A;
			float t2 = (B + det) / A;
			if (t2<0.0f || t1>tmax) return VCDR_CloudInter();
			return VCDR_CloudInter(clamp(0.0f, tmax, t1), clamp(0.0f, tmax, t2));
		}

		float map(vec3 v, float d) const
		{
			vec3 cloudsScale = 3.0f*0.9f*vec3(0.2f, 0.2f, 0.3f);
			float cloudsDensity = 0.01f;//0.01f;
			float w = 2.5f;// 3.0f*(0.5f*d + 0.5f);
			float hy = 1.0f;
			float n = cloudsDensity*FBM3D<>::fbmc<true>(cloudsScale*(v+vec3(0.0f, 0.0f, 6.0f)), 7, 0.5f, 4.0f)-0.001f;
			float distFromCenter = sqr(1.0f - d);
			return distFromCenter*hy*w*n;
		}


		vec3 render(vec3 ro, vec3 rd, float tmin, float tmax, vec3 col)
		{
			// integrate density
			float t = tmin;
			float delta = 0.1f;
			float integratedDensity = 0.0f;
			for (; t < tmax; t += delta)
			{
				vec3 pos = ro + t*rd;

				float d = max(0.0f, map(vec3(pos.x, pos.y, t-tmin), (pos-c).lengthsqr()/25.0f));
				integratedDensity = integratedDensity*(1.0f - d) + d;
				if (integratedDensity>1.0f) break;
				//delta = startingDelta;// +t*deltaScaling;
			}
			integratedDensity = min(integratedDensity, 1.0f);
			vec3 att = vec3(0.2f, 0.5f, 0.9f);
			//vec3 att = vec3(0.4, 0.6, 1.0f);
			float inv = (1.0f - integratedDensity);
			vec3 cl = 1.0f*mix(vec3(1.0f), inv*att, sqrtf(integratedDensity));
			if (integratedDensity < 0.1f) return mix(col, cl, integratedDensity*10.0f);
			return cl;//mix(col,vec3(1.0f), integratedDensity);
		}
	};
}

#endif