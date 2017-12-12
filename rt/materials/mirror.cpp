#include <rt/materials/mirror.h>
#include <core/color.h>

namespace rt
{
	RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		return RGBColor::rep(fresnelReflectance(dot(inDir, normal), eta, kappa));
	}
	RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return RGBColor::rep(0.0f);
	}
	Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		Vector inDir = reflect(outDir, normal);
		return SampleReflectance(inDir, getReflectance(texPoint, normal, outDir, inDir));
	}
}