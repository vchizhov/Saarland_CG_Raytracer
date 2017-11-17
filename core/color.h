#ifndef CG1RAYTRACER_RGBCOLOR_HEADER
#define CG1RAYTRACER_RGBCOLOR_HEADER

#include <core/macros.h>
#include <math.h>

namespace rt {

class Float4;

class ALIGN(16) RGBColor {
public:
	float r, g, b;

	RGBColor() {}
	RGBColor(float r, float g, float b) : r(r), g(g), b(b) {}
	explicit RGBColor(const Float4& f4);

	static RGBColor rep(float v) { return RGBColor(v, v, v); }

	RGBColor operator+(const RGBColor& c) const
	{
		return RGBColor(r + c.r, g + c.g, b + c.b);
	}
	RGBColor operator-(const RGBColor& c) const
	{
		return RGBColor(r - c.r, g - c.g, b - c.b);
	}
	RGBColor operator*(const RGBColor& c) const
	{
		return RGBColor(r*c.r, g*c.g, b*c.b);
	}
    
	bool operator==(const RGBColor& b) const
	{
		return r == b.r && g == b.g && this->b == b.b;
	}
	bool operator!=(const RGBColor& b) const
	{
		return r != b.r || g != b.g || this->b != b.b;
	}

	RGBColor clamp() const
	{
		return RGBColor(r < 0.0f ? 0.0f : (r > 1.0f ? 1.0f : r), g < 0.0f ? 0.0f : (g > 1.0f ? 1.0f : g), b < 0.0f ? 0.0f : (b > 1.0f ? 1.0f : b));
	}
	RGBColor gamma(float gam) const
	{
		return RGBColor(powf(r, gam), powf(g, gam), powf(b, gam));
	}
	float luminance() const
	{
		return 0.2126f*r + 0.7152f*g + 0.0722f*b;
	}



};

RGBColor operator*(float scalar, const RGBColor& b);
RGBColor operator*(const RGBColor& a, float scalar);
RGBColor operator/(const RGBColor& a, float scalar);

}

#endif
