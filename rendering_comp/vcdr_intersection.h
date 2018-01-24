#ifndef VCDR_INTERSECTION_HEADER
#define VCDR_INTERSECTION_HEADER

#include <rendering_comp/vcdr_vec.h>
namespace rt
{
	class VCDR_Solid;

	struct VCDR_Intersection
	{
		float t;
		const VCDR_Solid* obj;

		VCDR_Intersection() : t(FLT_MAX), obj(nullptr) {}
		VCDR_Intersection(float t, const VCDR_Solid* obj);

		operator bool() const
		{
			return  t < FLT_MAX;
		}

		bool operator<(const VCDR_Intersection& r)
		{
			return t < r.t;
		}
	};
}

#endif