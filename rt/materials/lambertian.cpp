#include <rt/materials/lambertian.h>
#include <core/color.h>

namespace rt
{
	RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		return dot(normal, inDir)*diffuse->getColor(texPoint) / pi;
	}
	RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return emission->getColor(texPoint);
	}
	Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return SampleReflectance();
	}
}