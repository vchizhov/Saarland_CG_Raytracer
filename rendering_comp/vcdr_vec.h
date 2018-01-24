#ifndef VCDR_VEC_HEADER
#define VCDR_VEC_HEADER
#include <rendering_comp/vcdr_util.h>
namespace rt
{
	struct vec2
	{
		float x, y;

		vec2() {}
		vec2(float s) : x(s), y(s) {}
		vec2(float x, float y) : x(x), y(y) {}

		vec2& operator+=(const vec2& r)
		{
			x += r.x;
			y += r.y;
			return *this;
		}

		vec2& operator*=(float r)
		{
			x *= r;
			y *= r;
			return *this;
		}
	};

	vec2 operator+(const vec2& l, const vec2& r);
	vec2 operator-(const vec2& l, const vec2& r);
	vec2 operator*(const vec2& l, const vec2& r);
	vec2 operator*(float l, const vec2& r);
	vec2 operator*(const vec2& l, float r);
	float dot(const vec2& l, const vec2& r);
	vec2 fract(const vec2& v);
	vec2 floor(const vec2& v);

	struct vec3
	{
		float x, y, z;

		vec3() {}
		vec3(float s) : x(s), y(s), z(s) {}
		vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		vec3(float x, vec2 yz) : x(x), y(yz.x), z(yz.y) {}
		vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

		vec3& operator=(const vec3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		vec3& operator+=(const vec3& r)
		{
			x += r.x;
			y += r.y;
			z += r.z;
			return *this;
		}

		vec3& operator*=(const vec3& r)
		{
			x *= r.x;
			y *= r.y;
			z *= r.z;
			return *this;
		}

		vec3& operator/=(float r)
		{
			x /= r;
			y /= r;
			z /= r;
			return *this;
		}

		float lengthsqr() const
		{
			return x*x + y*y + z*z;
		}

		float length() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		vec2 yz() const
		{
			return vec2(y, z);
		}
	};

	vec3 operator+(const vec3& l, const vec3& r);
	vec3 operator-(const vec3& l, const vec3& r);
	vec3 operator*(const vec3& l, const vec3& r);
	vec3 operator*(float l, const vec3& r);
	vec3 operator*(const vec3& l, float r);
	vec3 operator/(const vec3& l, float r);
	vec3 operator/(const vec3& l, const vec3& r);
	float dot(const vec3& l, const vec3& r);
	vec3 cross(const vec3& l, const vec3& r);
	vec3 floor(const vec3& v);
	vec3 normalize(const vec3& v);
	vec3 reflect(const vec3& l, const vec3& r);
	vec3 mix(const vec3& a, const vec3& b, float x);
	template<>
	vec3 clamp(vec3 a, vec3 b, vec3 v);
	vec3 sqrt(const vec3& v);

	struct vec4
	{
		float x, y, z, w;

		vec4() {}
		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		vec4(float x, vec3 yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
		vec4(const vec3& xyz, float w) : x(x), y(y), z(z), w(w) {}
		vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		vec3 yzw() const
		{
			return vec3(y, z, w);
		}

	};

	vec4 operator*(float l, const vec4& r);
}


#endif
