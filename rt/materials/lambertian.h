#ifndef CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER
#define CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {

class LambertianMaterial : public Material {
private:
	Texture* emission;
	Texture* diffuse;
public:
	LambertianMaterial(Texture* emission, Texture* diffuse) : emission(emission), diffuse(diffuse) {}
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
	virtual Sampling useSampling() const
	{
		// while we could weight the sampling by the cosine, it is not necessary since this is not a path tracer
		return SAMPLING_NOT_NEEDED;
	}
};

}

#endif
