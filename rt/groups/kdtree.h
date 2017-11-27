#ifndef CG1RAYTRACER_GROUPS_KDTREE_HEADER
#define CG1RAYTRACER_GROUPS_KDTREE_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt {

class KDTree : public Group {
public:
	KDTree() {}
    virtual BBox getBounds() const = 0;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const = 0;
	virtual void rebuildIndex() = 0;
	virtual ~KDTree() {}
	virtual void add(Primitive* p) = 0;
	virtual void setMaterial(Material* m) = 0;
	virtual void setCoordMapper(CoordMapper* cm) = 0;

};

}

#endif
