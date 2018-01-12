#include "interpolate.h"
#include "assert.h"

namespace rt {

    template <typename T>
    T lerp(const T& px0, const T& px1, float xPoint) {
		return px0 * (1.0f - xPoint) + px1  *xPoint;
    }

    template <typename T>
    T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
		float cWeight = 1.0f - (aWeight + bWeight);
		return a * aWeight + b * bWeight + c * cWeight;
    }

    template <typename T>
    T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
		// interpolate along x:
		T pxy0 = px0y0 * (1.0f - xWeight) + px1y0 * xWeight;
		T pxy1 = px0y1 * (1.0f - xWeight) + px1y1 * xWeight;
		// interpolate along y:
		return pxy0 * (1.0f - yWeight) + pxy1 * yWeight;
	}

    template <typename T>
    T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
        const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
        float xPoint, float yPoint, float zPoint) {
		// interpolate along x:
		T pxy0z0 = px0y0z0 * (1.0f - xPoint) + px1y0z0 * xPoint;
		T pxy0z1 = px0y0z1 * (1.0f - xPoint) + px1y0z1 * xPoint;
		T pxy1z0 = px0y1z0 * (1.0f - xPoint) + px1y1z0 * xPoint;
		T pxy1z1 = px0y1z1 * (1.0f - xPoint) + px1y1z1 * xPoint;
		// interpolate along y:
		T pxyz0 = pxy0z0 * (1.0f - yPoint) + pxy1z0 * yPoint;
		T pxyz1 = pxy0z1 * (1.0f - yPoint) + pxy1z1 * yPoint;
		// interpolate along z:
		return pxyz0 * (1.0f - zPoint) + pxyz1 * zPoint;
    }

}

