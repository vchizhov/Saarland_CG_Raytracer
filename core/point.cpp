#include <core/point.h>
#include <core/assert.h>
#include <core/float4.h>
namespace rt
{

	Point::Point(const Float4& f)
		: x(f.x/f.w), y(f.y/f.w), z(f.z/f.w)
	{
		Assert a(f.w != 0.0f);
		a << "Use a Float4 with w=0 in the Point ctor.";
	}

	Point operator*(const Float4& scale, const Point& p)
	{
		return Point(scale.x*p.x, scale.y*p.y, scale.z*p.z);
	}

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