#ifndef VCDR_INTEGRATOR_HEADER
#define VCDR_INTEGRATOR_HEADER
 
namespace rt
{
	class VCDR_World;
	class Ray;
	class RGBColor;
	class VCDR_Integrator
	{
	public:
		VCDR_Integrator(VCDR_World* world);
		virtual RGBColor getRadiance(Ray& ray) const = 0;
	private:
		VCDR_World* world;
	};
}

#endif
