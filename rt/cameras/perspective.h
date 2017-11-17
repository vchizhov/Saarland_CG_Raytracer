#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class PerspectiveCamera : public Camera {
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
public:
	/*!
	* Perspective camera constructor
	* 
	* @param[in]  center		the origin of the coordinate system for the camera
	* @param[in]  forward		the direction the camera's facing, effectively the unnormalized z basis vector of the coord. sys.
	* @param[in]  up			some up vector, may be the world's up or whatever, is not collinear and is not necessarily orthogonal to forward
	* @param[in]  phiMax		the maximum azimkuthal angle
	* @param[in]  thetaMax		the maximum polar angle
	* @param[in]  scale			a scale factor permitting spheroidal virtual screens
	*/
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle
        );

	/*!
	* Given (x,y) in [-1,1]^2 generates a ray by mapping (x,y) to the camera's virtual screen
	* 
	* @param[in]  x		normalized x pixel coordinate in [-1,1]
	* @param[in]  y		normalized and flipped y pixel coordinate in [-1,1]
	* 
	* @return			the generated ray based on (x,y)
	*/
    virtual Ray getPrimaryRay(float x, float y) const;
};

}


#endif
