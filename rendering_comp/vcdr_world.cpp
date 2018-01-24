#include <rendering_comp/vcdr_world.h>
#include <rendering_comp/vcdr_solid.h>

namespace rt
{
	void VCDR_World::add(VCDR_Solid* v)
	{
		elements.push_back(v);
	}

	VCDR_Intersection VCDR_World::intersect(vec3 ro, vec3 rd) const
	{
		VCDR_Intersection result;
		for (size_t i = 0; i < elements.size(); ++i)
		{
			VCDR_Intersection temp = elements[i]->intersect(ro, rd, 0.0f, result.t);
			if (temp < result) result = temp;
		}
		if (!result) result.obj = background;
		
		return result;
	}
}