#include <core/float4.h>
#include <core/point.h>
#include <core/vector.h>

namespace rt
{
	Float4::Float4(const Point& p)
		: x(p.x), y(p.y), z(p.z), w(1.0f)
	{

	}

	Float4::Float4(const Vector& v)
		: x(v.x), y(v.y), z(v.z), w(0.0f)
	{

	}

	Float4 operator*(float scalar, const Float4& b)
	{
		return Float4(scalar*b.x, scalar*b.y, scalar*b.z, scalar*b.w);
	}
	Float4 operator*(const Float4& a, float scalar)
	{
		return Float4(a.x*scalar, a.y*scalar, a.z*scalar, a.w*scalar);
	}
	Float4 operator/(const Float4& a, float scalar)
	{
		float invScalar = 1.0f / scalar;
		return Float4(a.x*invScalar, a.y*invScalar, a.z*invScalar, a.w*invScalar);
	}
	float dot(const Float4& a, const Float4& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	std::ostream& operator<<(std::ostream& os, const Float4& f)
	{
		os << f.x << "\t" << f.y << "\t" << f.z << "\t" << f.w << "\n";
		return os;
	}

	template<>
	Float4 min(const Float4& a, const Float4& b)
	{
		return Float4(a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y, a.z <= b.z ? a.z : b.z, a.w <= b.w ? a.w : b.w);
	}
	template<>
	Float4 max(const Float4& a, const Float4& b)
	{
		return Float4(a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y, a.z >= b.z ? a.z : b.z, a.w >= b.w ? a.w : b.w);
	}
}