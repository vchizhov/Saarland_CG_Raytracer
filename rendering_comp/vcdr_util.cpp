#include <rendering_comp/vcdr_util.h>


namespace rt
{
	float fract(float x)
	{
		return x - floorf(x);
	}

	float smoothstep(float a, float b, float x)
	{
		float t = clamp((x - a) / (b - a), 0.0f, 1.0f);
		return t * t * (3.0f - 2.0f * t);
	}
}
