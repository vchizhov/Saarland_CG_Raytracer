#ifndef CG1RAYTRACER_MATERIALS_FUZZYMIRROR_HEADER
#define CG1RAYTRACER_MATERIALS_FUZZYMIRROR_HEADER

#include <rt/materials/material.h>

namespace rt {

class FuzzyMirrorMaterial : public Material {
private:
	float eta, kappa, sinFuzz;
public:
	FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle) : eta(eta), kappa(kappa), sinFuzz(sinf(fuzzyangle)) {}
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const { return Material::SAMPLING_ALL; }
};

}

#endif
