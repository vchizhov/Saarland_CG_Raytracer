#include <rendering_comp/vcdr_intersection.h>
#include <rendering_comp/vcdr_solid.h>
namespace rt
{
	VCDR_Intersection::
		VCDR_Intersection(float t, const VCDR_Solid* obj) : t(t), obj(obj) {}
}