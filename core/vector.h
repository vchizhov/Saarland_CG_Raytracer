#ifndef CG1RAYTRACER_VECTOR_HEADER
#define CG1RAYTRACER_VECTOR_HEADER

#include <core/macros.h>
#include <core/scalar.h>//for min and max
#include <math.h>
#include <ostream>

namespace rt {
class Point;
class Float4;
class ALIGN(16) Vector {
public:
	union
	{
		struct {
			float x, y, z;
		};
		float e[3];
	};

    Vector() {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit Vector(const Float4& f4);

    static Vector rep(float v) { return Vector(v,v,v); }

	Vector operator+(const Vector& b) const
	{
		return Vector(x + b.x, y + b.y, z + b.z);
	}
	Vector operator-(const Vector& b) const
	{
		return Vector(x - b.x, y - b.y, z - b.z);
	}

	Vector normalize() const
	{
		float l = 1.0f/length();
		return Vector(x*l, y*l, z*l);
	}

    
	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}

	float lensqr() const
	{
		return x*x + y*y + z*z;
	}
	float length() const
	{
		return sqrtf(lensqr());
	}

	bool operator==(const Vector& b) const
	{
		return x == b.x && y == b.y && z == b.z;
	}
	bool operator!=(const Vector& b) const
	{
		return x != b.x || y != b.y || z != b.z;
	}

};

Vector operator*(float scalar, const Vector& b);
Vector operator*(const Vector& a, float scalar);
Vector operator/(const Vector& a, float scalar);
Vector cross(const Vector& a, const Vector& b);
float dot(const Vector& a, const Vector& b);

Vector min(const Vector& a, const Vector& b);
Vector max(const Vector& a, const Vector& b);

Vector compMul(const Vector& a, const Vector& b);

Point operator+(const Point& a, const Vector& b);

// what's vector + point supposed to mean geometrically?
Point operator+(const Vector& a, const Point& b);
Point operator-(const Point& a, const Vector& b);

std::ostream& operator<<(std::ostream& os, const Vector& v);

Vector operator-(const Point& a, const Point& b);

Vector perp(const Vector& a);

Vector reflect(const Vector& in, const Vector& norm);

template<>
Vector min(const Vector& a, const Vector& b);

template<>
Vector max(const Vector& a, const Vector& b);

}

#endif
