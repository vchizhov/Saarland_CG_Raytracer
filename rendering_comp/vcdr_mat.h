#ifndef VCDR_MAT_HEADER
#define VCDR_MAT_HEADER

#include <rendering_comp/vcdr_vec.h>


namespace rt
{
	struct mat2
	{
		union
		{
			struct
			{
				float m11, m12, m21, m22;
			};
			struct
			{
				vec2 v1, v2;
			};
		};

		mat2() {}

		mat2(float m11, float m12, float m21, float m22)
			: m11(m11), m12(m12), m21(m21), m22(m22) {}

		static mat2 identity()
		{
			return mat2(1.0f, 0.0f,
				0.0f, 1.0f);
		}
	};


	mat2 operator*(const mat2& l, const mat2& r);
	mat2 operator*(float l, const mat2& r);
	vec2 operator*(const mat2& l, const vec2& r);
	

	struct mat3
	{
		union
		{
			struct
			{
				float m11, m12, m13, m21, m22, m23, m31, m32, m33;
			};
			struct
			{
				vec3 v1, v2, v3;
			};
		};

		mat3() {}

		mat3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
			: m11(m11), m12(m12), m13(m13), m21(m21), m22(m22), m23(m23), m31(m31), m32(m32), m33(m33) {}

		mat3& operator=(const mat3& v)
		{
			v1 = v.v1;
			v2 = v.v2;
			v3 = v.v3;

			return *this;
		}

		mat3(const mat3& v)
			: v1(v.v1), v2(v.v2), v3(v.v3) {}

		static mat3 identity()
		{
			return mat3(1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f);
		}
	};

	mat3 operator*(const mat3& l, const mat3& r);
	mat3 operator*(float l, const mat3& r);
	vec3 operator*(const mat3& l, const vec3& r);
	
}

#endif


