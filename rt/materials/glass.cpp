#include <rt/materials/glass.h>
#include <rt/materials/mirror.h>

namespace rt
{
	RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		return RGBColor::rep(1.0f);
	}
	RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return RGBColor::rep(0.0f);
	}
	Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		float ceta;
		Vector norm = normal.normalize();
		float cs = clamp(-1.0f, 1.0f, dot(norm, outDir.normalize()));
		if ( cs < 0.0f)
		{
			ceta = eta;
			norm = -norm;
			cs = -cs;
		}
		else
		{
			ceta = 1.0f/eta;
		}
		float k = 1.0f - (1.0f-sqr(cs))*sqr(ceta);
		if (k < 0.0f) return SampleReflectance();// return SampleReflectance(reflect(outDir, norm), getReflectance(texPoint, norm, outDir, reflect(outDir, norm)));
		Vector refr = -ceta*outDir.normalize() + (ceta*cs - sqrtf(k))*norm;

		return SampleReflectance(refr.normalize(), RGBColor::rep(1.0f));
	}
}