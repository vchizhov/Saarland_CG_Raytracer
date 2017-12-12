#ifndef CG1RAYTRACER_MATERIALS_COOK_TORRANCE_HEADER
#define CG1RAYTRACER_MATERIALS_COOK_TORRANCE_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>

namespace rt {

	class CookTorranceMaterial : public Material {
	private:
		Texture* emission;
		Texture* diffuse;
		Texture* specular;
		// the anisotropy tangent could be passed as a texture:
		// if only we had a way to transform it when shading (we cannot since
		// we cannot pass a transformation matrix, because the interface of getReflecatnace does not permit it)
		//Texture* anisotropyTangent;
		float eta;
		// we use the fresnel term from Cook-Torrance's paper, we don't need kappa
		float kappa;
		float et, eb;
	public:
		CookTorranceMaterial(Texture* emission, Texture* diffuse, Texture* specular, float eta, float kappa, float et, float eb)
		: emission(emission), diffuse(diffuse), specular(specular), eta(eta), kappa(kappa), et(et), eb(eb) {}
		virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
		virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
		virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
		virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }
	};

}

#endif
