#ifndef CG1RAYTRACER_SCALAR_HEADER
#define CG1RAYTRACER_SCALAR_HEADER

#include <float.h>
#include <algorithm>
#include <cmath>

namespace rt {

// definition of min and max for arbitrary number of arguments
template<typename T, typename...Args>
T min(const T& a, const Args&...args)
{
	return min(a, min(args...));
}

template<typename T, typename...Args>
T max(const T& a, const Args&...args)
{
	return max(a, max(args...));
}

// base case for PODs, for more complex types like vector and point a specialization is necessary
template<typename T>
T min(const T& a, const T& b)
{
	return (a < b) ? a : b;
}

template<typename T>
T max(const T& a, const T& b)
{
	return (b < a) ? a : b;
}


// Clamps a value in the range [a,b]
template <typename T>
T clamp(T a, T b, T x) { return max(a, min(x, b)); }

// Computes a square of
template <typename T>
T sqr(const T& a) { return a*a; }

// Returns the fractional part of a number. For negative numbers it returns the negative fractional
float fractional(float f);

// Returns the difference between the given value and the nearest lower integer. Always in range [0, 1)
float absfractional(float f);

// The epsilon value adjustment for precision errors.
static const float epsilon = 0.000001f;

// The mathematical pi constant
static const float pi = 3.141592f;

typedef unsigned int uint;

}

#endif
