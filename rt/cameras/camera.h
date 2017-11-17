#ifndef CG1RAYTRACER_CAMERAS_CAMERA_HEADER
#define CG1RAYTRACER_CAMERAS_CAMERA_HEADER

namespace rt {

class Vector;
class Point;
class Ray;

class Camera {
protected:
public:
    virtual Ray getPrimaryRay(float x, float y) const = 0;
};

}

#endif

