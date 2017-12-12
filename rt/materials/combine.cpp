#include <rt/materials/combine.h>
#include <core/color.h>

namespace rt
{
	RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		RGBColor total = RGBColor::rep(0.0f);
		for (auto i = 0; i < materials.size(); ++i)
		{
			total = total + weights[i] * materials[i]->getReflectance(texPoint, normal, outDir, inDir);
		}
		// handle the case where the sampling material is also a combination
		if (samplingMaterial != nullptr && samplingMaterial->useSampling() == SAMPLING_SECONDARY)
			total = total + samplingMaterialWeight*samplingMaterial->getReflectance(texPoint, normal, outDir, inDir);
		return total;
	}
	RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		RGBColor total = RGBColor::rep(0.0f);
		for (auto i = 0; i < materials.size(); ++i)
		{
			total = total + weights[i] * materials[i]->getEmission(texPoint, normal, outDir);
		}
		if (samplingMaterial != nullptr)
			total = total + samplingMaterialWeight*samplingMaterial->getEmission(texPoint, normal, outDir);
		return total;
	}
	Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// we consider that at most 1 material requires special sampling
		// if we had more than 1, we would either need to split one ray into more than one, or choose between
		// the different possibilities with a random variable
		if(samplingMaterial!=nullptr)
		{
			SampleReflectance refl = samplingMaterial->getSampleReflectance(texPoint, normal, outDir);
			refl.reflectance = refl.reflectance * samplingMaterialWeight;
			return refl;
		}
		else
			return SampleReflectance();
	}
}