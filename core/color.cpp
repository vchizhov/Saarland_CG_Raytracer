#include <core/color.h>

namespace rt
{
	RGBColor operator*(float scalar, const RGBColor& b)
	{
		return RGBColor(scalar*b.r, scalar*b.g, scalar*b.b);
	}
	RGBColor operator*(const RGBColor& a, float scalar)
	{
		return RGBColor(a.r*scalar, a.g*scalar, a.b*scalar);
	}
	RGBColor operator/(const RGBColor& a, float scalar)
	{
		float is = 1.0f / scalar;
		return RGBColor(a.r*is, a.g*is, a.b*is);
	}
}