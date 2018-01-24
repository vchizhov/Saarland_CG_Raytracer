#ifndef VCDR_CAMERA_PERSPECTIVE
#define VCDR_CAMERA_PERSPECTIVE

#include <rendering_comp/vcdr_camera.h>

namespace rt
{
	class VCDR_CameraPerspective : public VCDR_Camera
	{
	private:
		float scaleFov;
		float aspectRatio;
	public:
		VCDR_CameraPerspective(vec3 o = vec3(0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0), vec3 forward = vec3(0.0f, 0.0f, 1.0f), float aspectRatio = 1.0f, float hfov = 0.5f*pi)
			: VCDR_Camera(o), aspectRatio(aspectRatio)
		{
			this->forward = normalize(forward);
			this->right = normalize(cross(up, forward));
			this->up = cross(forward, right);
			scaleFov = tanf(0.5f*hfov);
		}

		vec3 dir(float x, float y) const
		{
			return normalize(forward + x*scaleFov*right + y*scaleFov*aspectRatio*up);
		}
	};
}

#endif
