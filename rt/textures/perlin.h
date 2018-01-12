#ifndef CG1RAYTRACER_TEXTURES_PERLIN_HEADER
#define CG1RAYTRACER_TEXTURES_PERLIN_HEADER

#include <core/vector.h>

#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/scalar.h>
#include <core/interpolate.h>
#include <vector>
namespace rt {

    class PerlinTexture : public Texture {
	private:
		RGBColor white, black;
		std::vector<float> amplitudes;
		std::vector<float> frequencies;

		float valueNoise(float x, float y, float z) const;
    public:
		PerlinTexture(const RGBColor& white, const RGBColor& black)
			: white(white), black(black) {}
		void addOctave(float amplitude, float frequency)
		{
			amplitudes.push_back(amplitude);
			frequencies.push_back(frequency);
		}
		virtual RGBColor getColor(const Point& coord);
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
