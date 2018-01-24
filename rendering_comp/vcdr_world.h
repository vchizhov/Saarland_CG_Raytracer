#ifndef VCDR_WORLD_HEADER
#define VCDR_WORLD_HEADER


#include <rendering_comp/vcdr_misc.h>
#include <vector>
namespace rt
{
	struct VCDR_Intersection;
	class VCDR_Solid;
	class VCDR_World
	{
	private:
		std::vector<VCDR_Solid*> elements;
		VCDR_Solid* background;
	public:
		VCDR_SceneInfo info;

		VCDR_World(VCDR_SceneInfo info, VCDR_Solid* background) : info(info), background(background) {}

		void add(VCDR_Solid* v);

		VCDR_Intersection intersect(vec3 ro, vec3 rd) const;
	};
}

#endif
