#ifndef VCDR_UTIL_HEADER
#define VCDR_UTIL_HEADER
#include <core/scalar.h>
namespace rt
{
	template<typename T>
	struct Rect
	{
		T x0, y0, x1, y1;
		Rect() {}
		Rect(T x0, T y0, T x1, T y1) : x0(x0), y0(y0), x1(x1), y1(y1) {}
	};

	template<typename T>
	T mix(float x, T a, T b)
	{
		return a*(1.0f - x) + b*x;
	}

	float fract(float x);

}

#endif
