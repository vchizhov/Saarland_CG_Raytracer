#ifndef CG1RAYTRACER_MATERIALS_COMBINE_HEADER
#define CG1RAYTRACER_MATERIALS_COMBINE_HEADER

#include <rt/materials/material.h>
#include <rt/materials/combine.h>
#include <vector>

namespace rt {

class CombineMaterial : public Material {
private:
	std::vector<Material*> materials;
	std::vector<float> weights;
	Material* samplingMaterial;
	float samplingMaterialWeight;
public:
	CombineMaterial() : samplingMaterial(nullptr) {}
	// I assume the weights sum up to one - I do not check for this, nor do I do a renormalization
	// also this does not accept more than one material which requires special sampling
	void add(Material* material, float weight)
	{
		// update the sampling strategy
		switch (material->useSampling())
		{
		case SAMPLING_NOT_NEEDED:
			materials.push_back(material);
			weights.push_back(weight);
			break;
		case SAMPLING_ALL:
		case SAMPLING_SECONDARY:
			samplingMaterial = material;
			samplingMaterialWeight = weight;
			break;
		default:
			break;
		}
		
	}
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
	virtual Sampling useSampling() const
	{
		return (samplingMaterial == nullptr) ? SAMPLING_NOT_NEEDED : SAMPLING_SECONDARY;
	}
};

}

#endif
