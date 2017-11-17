#include <core/point.h>
#include <core/vector.h>

namespace rt
{
	Vector Point::operator-(const Point& b) const
	{
		return Vector(x - b.x, y - b.y, z - b.z);
	}

	Point operator*(float scalar, const Point& b)
	{
		return Point(scalar*b.x, scalar*b.y, scalar*b.z);
	}
	Point operator*(const Point& a, float scalar)
	{
		return Point(a.x*scalar, a.y*scalar, a.z*scalar);
	}

	Point min(const Point& a, const Point& b)
	{
		return Point(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
	}
	Point max(const Point& a, const Point& b)
	{
		return Point(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
	}
}