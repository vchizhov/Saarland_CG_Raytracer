#ifndef CG1RAYTRACER_MATERIALS_PHONG_HEADER
#define CG1RAYTRACER_MATERIALS_PHONG_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {

class PhongMaterial : public Material {
private:
	Texture* specular;
	float exponent;
public:
	PhongMaterial(Texture* specular, float exponent) : specular(specular), exponent(exponent) {}
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
	virtual Sampling useSampling() const
	{
		// sampling directions close to the refl. dir yields better results, but we are not path tracing
		return SAMPLING_NOT_NEEDED;
	}
};

}

#endif
