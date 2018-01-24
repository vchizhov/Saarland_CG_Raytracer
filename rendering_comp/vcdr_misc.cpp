#include <rendering_comp/vcdr_misc.h>

namespace rt
{
	vec3 fog(vec3 col, vec3 fogColor, float t, float att)
	{
		return mix(col, fogColor, 1.0f - expf(-att*t*t*t));
	}
}