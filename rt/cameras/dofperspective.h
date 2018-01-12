#ifndef CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class DOFPerspectiveCamera : public Camera {
private:
	//! origin of the camera's coord. sys.
	Point center;
	//! x basis vector of the camera's coord. sys.
	Vector right;
	//! y basis vector of the camera's coord. sys.
	Vector up;
	//! z basis vector of the camera's coord. sys.
	Vector forward;

	//! width of the virtual screen
	float scaleX;
	//! height of the virtual screen
	float scaleY;

	float focalDistance;
	float apertureRadius;
public:
	DOFPerspectiveCamera(
		const Point& center,
		const Vector& forward,
		const Vector& up,
		float verticalOpeningAngle,
		float horizonalOpeningAngle,
		float focalDistance,
		float apertureRadius
	);

	virtual Ray getPrimaryRay(float x, float y) const;

};

}


#endif
