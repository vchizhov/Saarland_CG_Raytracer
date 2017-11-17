#include <rt/cameras/orthographic.h>
#include <rt/ray.h>
namespace rt
{
	// I don't really have the option to preserve the aspect ratio
	// when I have scaleX and scaleY given
	OrthographicCamera::OrthographicCamera(
		const Point& center,
		const Vector& forward,
		const Vector& up,
		float scaleX,
		float scaleY
	)
		:center(center), forward(-forward.normalize()), scaleX(scaleX*0.5f), scaleY(scaleY*0.5f) //flipped z due to OpenGL conventions
	{
		// basisX = normalize(worldY x basisZ)
		right = cross(up, this->forward).normalize();
		// basisY = basisZ x basisX
		this->up = cross(this->forward, right);
	}

	// expects ndc coordinates
	Ray OrthographicCamera::getPrimaryRay(float x, float y) const
	{
		// since we are using the OpenGL convention, we actually need to take -z when calculating the ray's direction
		return Ray(center+x*scaleX*right+y*scaleY*up, -forward);
	}
}