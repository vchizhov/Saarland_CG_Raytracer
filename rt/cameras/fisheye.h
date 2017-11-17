#ifndef CG1RAYTRACER_CAMERAS_FISHEYE_HEADER
#define CG1RAYTRACER_CAMERAS_FISHEYE_HEADER

#include <rt/cameras/camera.h>
#include <core/scalar.h>
#include <rt/ray.h>

namespace rt {
	enum FISHEYE_MAPPING
	{
		// R = f*theta
		FISHEYE_EQUIDISTANT,
		// R = f*tan(theta)
		FISHEYE_STEREOGRAPHIC,
		FISHEYE_ORTHOGRAPHIC
	};
	class FisheyeCamera : public Camera {
	private:
		//! origin of the camera's coord. sys.
		Point center;
		//! x basis vector of the camera's coord. sys.
		Vector right;
		//! y basis vector of the camera's coord. sys.
		Vector up;
		//! z basis vector of the camera's coord. sys.
		Vector forward;

		//! the phi angle multiplier
		float phiFactor;
		//! r's divisor
		float thetaFactor;
		
		//! scale of the virtual screen
		Vector scale;

		//! fisheye mapping
		FISHEYE_MAPPING type;
		
	public:
		/*!
		* Fisheye camera constructor
		* where both areas may not overlap.
		* @param[in]  center		the origin of the coordinate system for the camera
		* @param[in]  forward		the direction the camera's facing, effectively the unnormalized z basis vector of the coord. sys.
		* @param[in]  up			some up vector, may be the world's up or whatever, is not collinear and is not necessarily orthogonal to forward
		* @param[in]  phiFactor		the phi angle multipler, should be in [0,1] if we don't want a wraparound
		* @param[in]  thetaFactor	the 1/f parameter for the various types of fisheye cameras
		* @param[in]  scale			a scale vector enabling spheroidal virtual screens
		*/
		FisheyeCamera(
			const Point& center,
			const Vector& forward,
			const Vector& up,
			FISHEYE_MAPPING type,
			float phiFactor = 1.0f,
			float thetaMax = 1.0f,
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
