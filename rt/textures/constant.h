#ifndef CG1RAYTRACER_TEXTURES_CONSTANT_HEADER
#define CG1RAYTRACER_TEXTURES_CONSTANT_HEADER

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>

namespace rt {

class ConstantTexture : public Texture {
private:
	RGBColor color;
public:
	ConstantTexture() {}
	ConstantTexture(const RGBColor& color) : color(color) {}
	virtual RGBColor getColor(const Point& coord) { return color; }
	virtual RGBColor getColorDX(const Point& coord) { return RGBColor::rep(0.0f); }
	virtual RGBColor getColorDY(const Point& coord) { return RGBColor::rep(0.0f); }
};

}

#endif
