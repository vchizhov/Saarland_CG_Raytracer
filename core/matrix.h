#ifndef CG1RAYTRACER_MATRIX_HEADER
#define CG1RAYTRACER_MATRIX_HEADER

#include <core/float4.h>
#include <core/vector.h>
namespace rt {

class Matrix {
public:

	union
	{
		struct {
			Float4 r1, r2, r3, r4;
		};
		Float4 e[4];
	};

    Matrix() {}
	Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4)
		:r1(r1), r2(r2), r3(r3), r4(r4) {}

	Float4& operator[](int idx) {
		return e[idx];
	}
	Float4 operator[](int idx) const {
		return e[idx];
	}

	Float4& col(int idx) { return Float4(r1[idx], r2[idx], r3[idx], r4[idx]); }
	Float4 col(int idx) const {return Float4(r1[idx], r2[idx], r3[idx], r4[idx]);}

	Matrix operator+(const Matrix& b) const { return Matrix(r1 + b.r1, r2 + b.r2, r3 + b.r3, r4 + b.r4); }
	Matrix operator-(const Matrix& b) const { return Matrix(r1 - b.r1, r2 - b.r2, r3 - b.r3, r4 - b.r4); }
    
	Matrix transpose() const
	{
		return Matrix(col(0), col(1), col(2), col(3));
	}
    Matrix invert() const;

	bool operator==(const Matrix& b) const { return r1 == b.r1 && r2 == b.r2 && r3 == b.r3 && r4 == b.r4; }
	bool operator!=(const Matrix& b) const { return r1 != b.r1 || r2 != b.r2 || r3 != b.r3 || r4 != b.r4; }

	Float4 operator*(const Float4& b) const;
	Vector operator*(const Vector& b) const;
    Point operator*(const Point& b) const;

	float adj00() const
	{
		return r2[1] * (r3[2] * r4[3] - r3[3] * r4[2])
			+ r2[2] * (r3[3] * r4[1] - r3[1] * r4[3])
			+ r2[3] * (r3[1] * r4[2] - r3[2] * r4[1]);
	}

	float adj01() const
	{
		return r2[0] * (r3[2] * r4[3] - r3[3] * r4[2])
			+ r2[2] * (r3[3] * r4[0] - r3[0] * r4[3])
			+ r2[3] * (r3[0] * r4[2] - r3[2] * r4[0]);
	}

	float adj02() const
	{
		return (r2[0] * (r3[1] * r4[3] - r3[3] * r4[1])
			+ r2[1] * (r3[3] * r4[0] - r3[0] * r4[3])
			+ r2[3] * (r3[0] * r4[1] - r3[1] * r4[0]));
	}

	float adj03() const
	{
		return r2[0] * (r3[1] * r4[2] - r3[2] * r4[1])
			+ r2[1] * (r3[2] * r4[0] - r3[0] * r4[2])
			+ r2[2] * (r3[0] * r4[1] - r3[1] * r4[0]);
	}

	float adj10() const
	{
		return  r1[1] * (r3[2] * r4[3] - r3[3] * r4[2])
			+ r1[2] * (r3[3] * r4[1] - r3[1] * r4[3])
			+ r1[3] * (r3[1] * r4[2] - r3[2] * r4[1]);
	}

	float adj11() const
	{
		return r1[0] * (r3[2] * r4[3] - r3[3] * r4[2])
			+ r1[2] * (r3[3] * r4[0] - r3[0] * r4[3])
			+ r1[3] * (r3[0] * r4[2] - r3[2] * r4[0]);
	}

	float adj12() const
	{
		return (r1[0] * (r3[1] * r4[3] - r3[3] * r4[1])
			+ r1[1] * (r3[3] * r4[0] - r3[0] * r4[3])
			+ r1[3] * (r3[0] * r4[1] - r3[1] * r4[0]));
	}

	float adj13() const
	{
		return r1[0] * (r3[1] * r4[2] - r3[2] * r4[1])
			+ r1[1] * (r3[2] * r4[0] - r3[0] * r4[2])
			+ r1[2] * (r3[0] * r4[1] - r3[1] * r4[0]);
	}

	float det() const
	{
		float a1 = r1[0] * adj00();
		float a2 = r1[1] * adj01();
		float a3 = r1[2] * adj02();
		float a4 = r1[3] * adj03();
		return a1 - a2 + a3 - a4;
	}

	static Matrix zero() { return Matrix(Float4::rep(0.0f), Float4::rep(0.0f), Float4::rep(0.0f), Float4::rep(0.0f)); }
	static Matrix identity() {
		return Matrix(Float4(1.0f, 0.0f, 0.0f, 0.0f), Float4(0.0f, 1.0f, 0.0f, 0.0f),
			Float4(0.0f, 0.0f, 1.0f, 0.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix system(const Vector& e1, const Vector& e2, const Vector& e3)
	{
		return Matrix(Float4(e1), Float4(e2), Float4(e3), Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix translation(const Vector& v)
	{
		return Matrix(Float4(1.0f, 0.0f, 0.0f, v.x), Float4(0.0f, 1.0f, 0.0f, v.y),
			Float4(0.0f, 0.0f, 1.0f, v.z), Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix scaling(float scalar)
	{
		return Matrix(Float4(scalar, 0.0f, 0.0f, 0.0f), Float4(0.0f, scalar, 0.0f, 0.0f),
			Float4(0.0f, 0.0f, scalar, 0.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix scaling(const Vector& v)
	{
		return Matrix(Float4(v.x, 0.0f, 0.0f, 0.0f), Float4(0.0f, v.y, 0.0f, 0.0f),
			Float4(0.0f, 0.0f, v.z, 0.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	// expects |axis| = 1
	static Matrix rotation(const Vector& axis, float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		return Matrix(Float4(x*x*(1.0f - c) + c, x*y*(1.0f - c) - z*s, x*z*(1.0f - c) + y*s, 0.0f),
			Float4(x*y*(1.0f - c) + z*s, c + y*y*(1.0f - c), y*z*(1.0f - c) - x*s, 0.0f),
			Float4(x*z*(1.0f - c) - y*s, y*z*(1.0f - c) + x*s, c + z*z*(1 - c), 0.0f),
			Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}
};

Matrix product(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, float scalar);
Matrix operator*(float scalar, const Matrix& a);
std::ostream& operator<<(std::ostream& os, const Matrix& m);
}


#endif
