#include <rt/cameras/environment.h>

namespace rt
{
	EnvironmentCamera::EnvironmentCamera(const Point& center,
		const Vector& forward,
		const Vector& up,
		float phiMax,
		float thetaMax,
		const Vector& scale
	) : center(center), forward(-forward.normalize()), phiMax(0.5f*phiMax), thetaMax(0.5f*thetaMax), scale(scale) //flipped z due to OpenGL's convention
	{
		// basisX = normalize(worldY x basisZ)
		right = cross(up, this->forward).normalize();
		// basisY = basisZ x basisX
		this->up = cross(this->forward, right);
	}

	Ray EnvironmentCamera::getPrimaryRay(float x, float y) const
	{
		float theta = 0.5f*pi - thetaMax*y;
		float phi = pi - phiMax*x;
		
		Vector dir = sinf(theta)*cosf(phi)*right + sinf(theta)*sinf(phi)*up - cosf(theta)*forward; //flipped z due to OpenGL's convention
		dir = compMul(dir, scale);
		return Ray(center, dir.normalize());
	}
}