#include <rendering_comp/vcdr_mat.h>

namespace rt
{
	///////////////////////////////////////////////////////////////////////
	//                             MAT2                                  //
	///////////////////////////////////////////////////////////////////////
	mat2 operator*(const mat2& l, const mat2& r)
	{
		return mat2(l.m11*r.m11 + l.m12*r.m21, l.m11*r.m12 + l.m12*r.m22,
			l.m21*r.m11 + l.m22*r.m21, l.m21*r.m12 + l.m22*r.m22);
	}
	mat2 operator*(float l, const mat2& r)
	{
		return mat2(l*r.m11, l*r.m12,
			l*r.m21, l*r.m22);
	}
	vec2 operator*(const mat2& l, const vec2& r)
	{
		return vec2(dot(l.v1, r), dot(l.v2, r));
	}

	///////////////////////////////////////////////////////////////////////
	//                             MAT3                                  //
	///////////////////////////////////////////////////////////////////////
	mat3 operator*(const mat3& l, const mat3& r)
	{
		return mat3(l.m11*r.m11 + l.m12*r.m21 + l.m13*r.m31, l.m11*r.m12 + l.m12*r.m22 + l.m13*r.m32, l.m13*r.m13 + l.m12*r.m23 + l.m13*r.m33,
			l.m21*r.m11 + l.m22*r.m21 + l.m23*r.m31, l.m21*r.m12 + l.m22*r.m22 + l.m23*r.m32, l.m21*r.m13 + l.m22*r.m23 + l.m23*r.m33,
			l.m31*r.m11 + l.m32*r.m21 + l.m33*r.m31, l.m31*r.m12 + l.m32*r.m22 + l.m33*r.m32, l.m31*r.m13 + l.m32*r.m23 + l.m33*r.m33);
	}
	mat3 operator*(float l, const mat3& r)
	{
		return mat3(l*r.m11, l*r.m12, l*r.m13,
			l*r.m21, l*r.m22, l*r.m23,
			l*r.m31, l*r.m32, l*r.m33);
	}
	vec3 operator*(const mat3& l, const vec3& r)
	{
		return vec3(dot(l.v1, r), dot(l.v2, r), dot(l.v3, r));
	}
	
}