#include <rt/materials/fuzzymirror.h>
#include <rt/materials/mirror.h>
#include <core/random.h>

namespace rt
{
	RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		return RGBColor::rep(fresnelReflectance(dot(inDir, normal), eta, kappa));
	}
	RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return RGBColor::rep(0.0f);
	}
	Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		Vector inDir = reflect(outDir, normal);
		// generate uniformly distributed points on a disk
		Vector t = perp(inDir).normalize();
		Vector b = cross(inDir, t);
		float r1 = sqrtf(random())*sinFuzz;
		float r2 = 2.0f*pi*random();
		Vector p = inDir + cosf(r2)*r1*t + sinf(r2)*r1*b;
		return SampleReflectance(p.normalize(), getReflectance(texPoint, normal, outDir, inDir));
	}
}