#include <core/vector.h>
#include <core/point.h>

namespace rt
{
	Vector operator*(float scalar, const Vector& b)
	{
		return Vector(scalar*b.x, scalar*b.y, scalar*b.z);
	}
	Vector operator*(const Vector& a, float scalar)
	{
		return Vector(a.x*scalar, a.y*scalar, a.z*scalar);
	}
	Vector operator/(const Vector& a, float scalar)
	{
		float is = 1.0f / scalar;
		return Vector(a.x*is, a.y*is, a.z*is);
	}
	Vector cross(const Vector& a, const Vector& b)
	{
		return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	float dot(const Vector& a, const Vector& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	Vector min(const Vector& a, const Vector& b)
	{
		return Vector(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
	}
	Vector max(const Vector& a, const Vector& b)
	{
		return Vector(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
	}

	Vector compMul(const Vector& a, const Vector& b)
	{
		return Vector(a.x*b.x, a.y*b.y, a.z*b.z);
	}

	Point operator+(const Point& a, const Vector& b)
	{
		return Point(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// what's vector + point supposed to mean geometrically?
	Point operator+(const Vector& a, const Point& b)
	{
		return Point(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	Point operator-(const Point& a, const Vector& b)
	{
		return Point(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	std::ostream& operator<<(std::ostream& os, const Vector& v)
	{
		os << "(" << v.x << "," << v.y << "," << v.z << ")";
		return os;
	}

	Vector operator-(const Point& a, const Point& b)
	{
		return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	Vector perp(const Vector& a)
	{
		float fx = fabsf(a.x);
		float fy = fabsf(a.y);
		float fz = fabsf(a.z);
		if ((fx > fy && fx > fz) || (fy > fx && fy > fz))
			return Vector(a.y, -a.x, 0.0f);
		else if (fz != 0.0f)
			return Vector(-a.z, 0.0f, a.x);
		else
			return Vector::rep(0.0f);
	}

	template<>
	Vector min(const Vector& a, const Vector& b)
	{
		return Vector(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
	}

	template<>
	Vector max(const Vector& a, const Vector& b)
	{
		return Vector(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
	}
}