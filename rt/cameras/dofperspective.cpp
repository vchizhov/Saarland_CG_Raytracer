#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <core/random.h>

namespace rt
{
	DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center,
		const Vector& forward,
		const Vector& up,
		float verticalOpeningAngle,
		float horizontalOpeningAngle,
		float focalDistance,
		float apertureRadius
	) : center(center), forward(-forward.normalize()), focalDistance(focalDistance),  apertureRadius(apertureRadius)
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
	Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const
	{
		// we use -z like in OpenGL
		Point p = center -forward*focalDistance + x*scaleX*right + y*scaleY*up;
		float r1 = sqrtf(random())*apertureRadius;
		float r2 = random()*2.0f*pi;
		Point center2 = center + right*r1*cosf(r2) + up*r1*sinf(r2);
		return Ray(center2, (p-center2).normalize());
	}
}