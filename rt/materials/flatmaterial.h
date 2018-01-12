#ifndef CG1RAYTRACER_MATERIALS_FLATMATERIAL_HEADER
#define CG1RAYTRACER_MATERIALS_FLATMATERIAL_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {

class FlatMaterial : public Material {
private:
	Texture* texture;
public:
	FlatMaterial(Texture* texture) : texture(texture) {}
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }
};

}

#endif
