#ifndef CG1RAYTRACER_FLOAT4_HEADER
#define CG1RAYTRACER_FLOAT4_HEADER

#include <core/macros.h>
#include <core/scalar.h>


namespace rt {

class Point;
class Vector;

class ALIGN(16) Float4 {
public:

	union
	{
		struct {
			float x, y, z, w;
		};
		float e[4];
	};

    Float4() {}
	Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	explicit Float4(const Point& p);
    explicit Float4(const Vector& v);

    static Float4 rep(float v) { return Float4(v,v,v,v); }

	float& operator[](int idx) {
		return e[idx];
	}
	float operator[](int idx) const {
		return e[idx];
	}

	Float4 operator+(const Float4& b) const { return Float4(x + b.x, y + b.y, z + b.z, w + b.w); }
	Float4 operator-(const Float4& b) const { return Float4(x - b.x, y - b.y, z - b.z, w - b.w); }
	Float4 operator*(const Float4& b) const { return Float4(x * b.x, y * b.y, z * b.z, w * b.w); }
	Float4 operator/(const Float4& b) const {return Float4(x / b.x, y / b.y, z / b.z, w / b.w); }


	Float4 operator-() const { return Float4(-x, -y, -z, w); }

	bool operator==(const Float4& b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }
	bool operator!=(const Float4& b) const { return x != b.x || y != b.y || z != b.z || w != b.w; }

};

Float4 operator*(float scalar, const Float4& b);
Float4 operator*(const Float4& a, float scalar);
Float4 operator/(const Float4& a, float scalar);
float dot(const Float4& a, const Float4& b);
std::ostream& operator<<(std::ostream& os, const Float4& f);

template<>
Float4 min(const Float4& a, const Float4& b);
template<>
Float4 max(const Float4& a, const Float4& b);


}

#endif
