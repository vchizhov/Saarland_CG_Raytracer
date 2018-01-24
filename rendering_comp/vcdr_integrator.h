#ifndef VCDR_INTEGRATOR_HEADER
#define VCDR_INTEGRATOR_HEADER
#include <rendering_comp/vcdr_vec.h>
#include <rendering_comp/vcdr_world.h>
namespace rt
{
	class VCDR_Integrator
	{
	public:
		virtual vec3 getColor(vec3 ro, vec3 rd, const VCDR_World* world) const = 0;
	};
}

#endif
