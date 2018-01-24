#ifndef VCDR_SOLID_HEADER
#define VCDR_SOLID_HEADER

#include <rendering_comp/vcdr_intersection.h>
#include <rendering_comp/vcdr_world.h>
namespace rt
{
	class VCDR_Solid
	{
	public:

		VCDR_Solid() {}

		virtual float map(vec3 pos) const
		{
			return 0.0f;
		}

		

		virtual VCDR_Intersection intersect(vec3 ro, vec3 rd, float tmin = 0.0f, float tmax = 30.0f) const = 0;

		virtual vec3 render(vec3 ro, vec3 rd, float t, const VCDR_World* world) const = 0;

		virtual ~VCDR_Solid()
		{

		}
		
	};
}

#endif
