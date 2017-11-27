#include <core/point.h>

namespace rt
{
	Point operator*(float scalar, const Point& b)
	{
		return Point(scalar*b.x, scalar*b.y, scalar*b.z);
	}
	Point operator*(const Point& a, float scalar)
	{
		return Point(a.x*scalar, a.y*scalar, a.z*scalar);
	}
	template<>
	Point min(const Point& a, const Point& b)
	{
		return Point(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
	}

	template<>
	Point max(const Point& a, const Point& b)
	{
		return Point(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
	}

}