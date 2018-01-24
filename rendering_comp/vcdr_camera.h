#ifndef VCDR_CAMERA_HEADER
#define VCDR_CAMERA_HEADER
#include <rendering_comp/vcdr_vec.h>
namespace rt
{
	class VCDR_Camera
	{
	protected:
		vec3 o;
		vec3 right, up, forward;
	public:

		VCDR_Camera(vec3 o) : o(o) {}

		vec3 origin() const
		{
			return o;
		}

		// x,y in [-1,1] (ndc)
		virtual vec3 dir(float x, float y) const = 0;
	};
}

#endif
