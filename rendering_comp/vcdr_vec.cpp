#include <rendering_comp/vcdr_vec.h>
#include <rendering_comp/vcdr_util.h>
namespace rt
{
	///////////////////////////////////////////////////////////////////////
	//                             VEC2                                  //
	///////////////////////////////////////////////////////////////////////
	vec2 operator+(const vec2& l, const vec2& r)
	{
		return vec2(l.x + r.x, l.y + r.y);
	}
	vec2 operator-(const vec2& l, const vec2& r)
	{
		return vec2(l.x - r.x, l.y - r.y);
	}
	vec2 operator*(const vec2& l, const vec2& r)
	{
		return vec2(l.x*r.x, l.y*r.y);
	}
	vec2 operator*(float l, const vec2& r)
	{
		return vec2(l*r.x, l*r.y);
	}
	vec2 operator*(const vec2& l, float r)
	{
		return vec2(l.x*r, l.y*r);
	}
	vec2 fract(const vec2& v)
	{
		return vec2(fract(v.x), fract(v.y));
	}
	vec2 floor(const vec2& v)
	{
		return vec2(floorf(v.x), floorf(v.y));
	}
	float dot(const vec2& l, const vec2& r)
	{
		return l.x*r.x + l.y*r.y;
	}
	///////////////////////////////////////////////////////////////////////
	//                             VEC3                                  //
	///////////////////////////////////////////////////////////////////////
	vec3 operator+(const vec3& l, const vec3& r)
	{
		return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
	}
	vec3 operator-(const vec3& l, const vec3& r)
	{
		return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
	}
	vec3 operator*(const vec3& l, const vec3& r)
	{
		return vec3(l.x*r.x, l.y*r.y, l.z*r.z);
	}
	vec3 operator*(float l, const vec3& r)
	{
		return vec3(l*r.x, l*r.y, l*r.z);
	}
	vec3 operator*(const vec3& l, float r)
	{
		return vec3(l.x*r, l.y*r, l.z*r);
	}
	vec3 operator/(const vec3& l, float r)
	{
		return vec3(l.x / r, l.y / r, l.z / r);
	}
	vec3 operator/(const vec3& l, const vec3& r)
	{
		return vec3(l.x / r.x, l.y / r.y, l.z / r.z);
	}
	float dot(const vec3& l, const vec3& r)
	{
		return l.x*r.x + l.y*r.y + l.z*r.z;
	}
	vec3 cross(const vec3& l, const vec3& r)
	{
		return vec3(l.y*r.z - l.z*r.y, l.z*r.x - l.x*r.z, l.x*r.y - l.y*r.x);
	}
	vec3 floor(const vec3& v)
	{
		return vec3(floorf(v.x), floorf(v.y), floorf(v.z));
	}
	vec3 normalize(const vec3& v)
	{
		return v / v.length();
	}
	vec3 reflect(const vec3& l, const vec3& r)
	{
		return l - 2.0f*dot(r, l)*r;
	}
	vec3 mix(const vec3& a, const vec3& b, float x)
	{
		return (1.0f - x)*a + x*b;
	}
	template<>
	vec3 clamp(vec3 a, vec3 b, vec3 v)
	{
		return vec3(clamp(a.x, b.x, v.x), clamp(a.x, b.x, v.y), clamp(a.x, b.x, v.z));
	}
	vec3 sqrt(const vec3& v)
	{
		return vec3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z));
	}
	///////////////////////////////////////////////////////////////////////
	//                             VEC4                                  //
	///////////////////////////////////////////////////////////////////////
	vec4 operator*(float l, const vec4& r)
	{
		return vec4(l*r.x, l*r.y, l*r.z, l*r.w);
	}
}