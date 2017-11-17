#include <rt/cameras/fisheye.h>

namespace rt
{
	FisheyeCamera::FisheyeCamera(const Point& center,
		const Vector& forward,
		const Vector& up,
		FISHEYE_MAPPING type,
		float phiFactor,
		float thetaFactor,
		const Vector& scale
	) : center(center), forward(-forward.normalize()), type(type), phiFactor(phiFactor), thetaFactor(thetaFactor), scale(scale) //flipped z due to OpenGL's convention
	{
		// basisX = normalize(worldY x basisZ)
		right = cross(up, this->forward).normalize();
		// basisY = basisZ x basisX
		this->up = cross(this->forward, right);
	}

	Ray FisheyeCamera::getPrimaryRay(float x, float y) const
	{
		float rSq = x*x + y*y;
		// a hack to avoid wrapping around:
		if (rSq > 1.0f)
			return Ray(center, Vector(0.0f, 0.0f, 0.0f));
		float phi = phiFactor*atan2f(y,x);
		float r = sqrtf(rSq);
		float theta;
		switch(type)
		{
		case FISHEYE_EQUIDISTANT:
			// equidistant: R = f*theta -> theta = R/f
			theta = thetaFactor*0.5f*pi*r;
			break;
		case FISHEYE_STEREOGRAPHIC:
			// stereographic: R = f*tan(theta) -> theta = atanf(r/f)
			theta = 2.0f*atanf(0.5f*thetaFactor*r);
			break;
		case FISHEYE_ORTHOGRAPHIC:
			// orhographic: R=2f*sin(theta/2) -> theta = 2*asinf(R/2f), I simplified it to make it more intuitive:
			theta = 2.0f*asinf(0.5f*thetaFactor*r);
		};
		Vector dir = sinf(theta)*cos(phi)*right + sinf(theta)*sin(phi)*up - cosf(theta)*forward; //flipped z due to OpenGL's convention
		dir = compMul(dir, scale);
		return Ray(center, dir.normalize());
	}
}


