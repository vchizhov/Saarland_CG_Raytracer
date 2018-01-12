#ifndef CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER
#define CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/point.h>

namespace rt {

    class CheckerboardTexture : public Texture {
	private:
		RGBColor white, black;
    public:
		CheckerboardTexture(const RGBColor& white, const RGBColor& black) : white(white), black(black) {}
		virtual RGBColor getColor(const Point& coord)
		{
			int lu = (int)floorf(2.0f*coord.x);
			int lv = (int)floorf(2.0f*coord.y);
			int lw = (int)floorf(2.0f*coord.z);

			return (lu + lv + lw) % 2 == 0 ? white : black;
		}
		virtual RGBColor getColorDX(const Point& coord)
		{
			return RGBColor::rep(0.0f);
		}
		virtual RGBColor getColorDY(const Point& coord)
		{
			return RGBColor::rep(0.0f);
		}

    };

}

#endif
