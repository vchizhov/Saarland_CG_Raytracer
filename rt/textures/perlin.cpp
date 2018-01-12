#include <rt/textures/perlin.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/scalar.h>

namespace rt {

namespace {

    /* returns a value in range -1 to 1 */
    float noise(int x, int y, int z) {
        int n = x + y * 57 + z * 997;
        n = (n<<13) ^ n;
        return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

	

}

float PerlinTexture::valueNoise(float x, float y, float z) const
{
	int x0 = (int)floorf(x);
	int y0 = (int)floorf(y);
	int z0 = (int)floorf(z);

	return lerp3d(noise(x0, y0, z0), noise(x0 + 1, y0, z0), noise(x0, y0 + 1, z0), noise(x0 + 1, y0 + 1, z0), noise(x0, y0, z0 + 1), noise(x0 + 1, y0, z0 + 1),
		noise(x0, y0 + 1, z0 + 1), noise(x0 + 1, y0 + 1, z0 + 1), x - floorf(x), y - floorf(y), z - floorf(z));
}

RGBColor PerlinTexture::getColor(const Point& coord)
{
	float noiseSum = 0.0f;
	for (size_t i = 0; i < amplitudes.size(); ++i)
	{
		noiseSum += amplitudes[i] * valueNoise(frequencies[i] * coord.x, frequencies[i] * coord.y, frequencies[i] * coord.z);
	}

	float w = clamp(0.0f, 1.0f, 0.5f + 0.5f*noiseSum);
	return lerp(black, white, w);
}

}
