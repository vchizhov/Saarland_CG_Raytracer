#ifndef CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER
#define CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER

#include <core/vector.h>
#include <core/image.h>
#include <rt/textures/texture.h>
#include <core/interpolate.h>
#include <core/point.h>

namespace rt {

class ImageTexture : public Texture {
public:
    enum BorderHandlingType {
        CLAMP,
        MIRROR,
        REPEAT
    };

    enum InterpolationType {
        NEAREST,
        BILINEAR
    };

private:
	Image image;
	BorderHandlingType bh;
	InterpolationType i;

public:
	ImageTexture() {}
    ImageTexture(const Image& image, BorderHandlingType bh=REPEAT, InterpolationType i=BILINEAR)
		:image(image), bh(bh), i(i)
	{}
	ImageTexture(const std::string& filename, BorderHandlingType bh = REPEAT, InterpolationType i = BILINEAR)
		:bh(bh), i(i)
	{
		image.readPNG(filename);
	}
	virtual RGBColor getColor(const Point& coord)
	{
		float cx = coord.x;
		float cy = coord.y;
		if (cx > 1.0f || cx < 0.0f)
		{
			switch (bh)
			{
			case CLAMP:
				cx = clamp(0.0f, 1.0f, cx);
				break;
			case REPEAT:
				cx = cx - floorf(cx);
				break;
			case MIRROR:
				cx = cx - floorf(cx);
				if (int(floorf(cx)) % 2 == 1)
					cx = 1.0f - cx;
				break;
			}
		}
		if (cy > 1.0f || cy < 0.0f)
		{
			switch (bh)
			{
			case CLAMP:
				cy = clamp(0.0f, 1.0f, cy);
				break;
			case REPEAT:
				cy = cy - floorf(cy);
				break;
			case MIRROR:
				cy = cy - floorf(cy);
				if (int(floorf(cy)) % 2 == 1)
					cy = 1.0f - cy;
				break;
			}
		}

		RGBColor color;
		switch (i)
		{
		case NEAREST:
		{
			uint x = min((uint)floorf(cx*image.width()), image.width() - 1);
			uint y = min((uint)floorf(cy*image.height()), image.height() - 1);
			color = image(x, y);
			break;
		}
		case BILINEAR:
		{
			float u = cx*(image.width() - 1);
			float v = cy*(image.height() - 1);
			uint x = (uint)floorf(u);
			uint y = (uint)floorf(v);
			float fu = u - floorf(u);
			float fv = v - floorf(v);
			
			color = lerp2d(image(x, y), image(min(x + 1, image.width()-1), y), image(x, min(y + 1, image.height()-1)), 
				image(min(x + 1,image.width()-1), min(y + 1, image.height()-1)), fu, fv);
			break;
		}
		}
		return color;
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
