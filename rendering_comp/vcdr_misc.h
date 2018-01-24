#ifndef VCDR_MISC_HEADER
#define VCDR_MISC_HEADER
#include <rendering_comp/vcdr_vec.h>
namespace rt
{
	struct VCDR_Sun
	{
		vec3 dir;
		vec3 col;
		vec3 glareCol;
		float size;
		float glareSize;
	};

	struct VCDR_SceneInfo
	{
		VCDR_Sun sun;
		float skyHeight;
		vec3 fogColor;
		float fogAttenuation;
		vec3 lightColor;
		vec3 domeLight;
		vec3 domeLightDir;
	};

	// fog based on Beer's law
	vec3 fog(vec3 col, vec3 fogColor, float t, float att = 0.00005f);
}

#endif
