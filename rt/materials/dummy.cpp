#include <rt/materials/dummy.h>
#include <core/color.h>
namespace rt
{
	RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		// dot(normal,inDir)>0.0f - we do check for that in the integrator
		return RGBColor::rep(dot(normal, inDir));
	}
	RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// no emission
		return RGBColor::rep(0.0f);
	}
	Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// we don't need to sample anything - just return whatever
		return SampleReflectance();
	}
}