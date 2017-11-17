#ifndef CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER

#include <rt/cameras/camera.h>
#include <core/scalar.h>
#include <rt/ray.h>

namespace rt {

	class EnvironmentCamera : public Camera {
	private:
		//! origin of the camera's coord. sys.
		Point center;
		//! x basis vector of the camera's coord. sys.
		Vector right;
		//! y basis vector of the camera's coord. sys.
		Vector up;
		//! z basis vector of the camera's coord. sys.
		Vector forward;

		//! the maximum azimuthal angle
		float phiMax;
		//! the maximum polar angle
		float thetaMax;

		//! scale of the virtual screen
		Vector scale;
	public:
		/*!
		* vironment camera constructor
		* where both areas may not overlap.
		* @param[in]  center		the origin of the coordinate system for the camera
		* @param[in]  forward		the direction the camera's facing, effectively the unnormalized z basis vector of the coord. sys.
		* @param[in]  up			some up vector, may be the world's up or whatever, is not collinear and is not necessarily orthogonal to forward
		* @param[in]  phiMax		the maximum azimuthal angle
		* @param[in]  thetaMax		the maximum polar angle
		* @param[in]  scale			a scale vector enabling spheroidal virtual screens
		*/
		EnvironmentCamera(
			const Point& center,
			const Vector& forward,
			const Vector& up,
			float phiMax = 2.0f*pi,
			float thetaMax = pi,
			const Vector& scale = Vector(1.0f, 1.0f, 1.0f)
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
