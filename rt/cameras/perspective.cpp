#include <rt/cameras/perspective.h>
#include <rt/ray.h>

namespace rt
{
	PerspectiveCamera::PerspectiveCamera(const Point& center,
		const Vector& forward,
		const Vector& up,
		float verticalOpeningAngle,
		float horizontalOpeningAngle
	) : center(center), forward(-forward.normalize()) //the minus here is due to OpenGL's flipped z axis
	{

		// basisX = normalize(worldY x basisZ)
		right = cross(up, this->forward).normalize();
		// basisY = basisZ x basisX
		this->up = cross(this->forward, right);
		// we fix the distance from the center to the screen to 1, then we can calculate the scale:
		scaleX = tanf(horizontalOpeningAngle*0.5f);
		scaleY = tanf(verticalOpeningAngle*0.5f);
	}

	//expects ndc coordinates
	Ray PerspectiveCamera::getPrimaryRay(float x, float y) const
	{
		// we use -z like in OpenGL
		Vector dir = -forward + x*scaleX*right + y*scaleY*up;
		return Ray(center+dir, dir.normalize());
	}
}