#include <rt/materials/phong.h>
#include <core/color.h>

namespace rt
{
	RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		
		float cosLobe = powf(max(0.0f, dot(outDir, reflect(inDir, normal))), exponent);
		// ks*(R*V)^exponent
		return  dot(inDir, normal) * cosLobe*specular->getColor(texPoint) * (exponent + 2.0f) * 0.5f / pi;
	}
	RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// no emission
		return RGBColor::rep(0.0f);
	}
	Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// no sampling
		return SampleReflectance();
	}
}