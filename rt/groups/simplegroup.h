#ifndef CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER
#define CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
namespace rt {

class SimpleGroup : public Group {
private:
	std::vector<Primitive*> primitives;
	BBox bbox;
public:
	
	virtual ~SimpleGroup()
	{
		//for (uint i = 0; i < primitives.size(); ++i)
		//	delete primitives[i];
	}
	virtual BBox getBounds() const
	{
		return bbox;
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		Intersection result;
		result.distance = previousBestDistance;
		bool hit = false;
		Intersection temp;
		for (uint i = 0; i < primitives.size(); ++i)
		{
			temp = primitives[i]->intersect(ray, result.distance);
			if (temp)
			{
				if (!hit) hit = true;
				result = temp;
			}
		}
		if (hit == false) result.distance = -1.0f;
		return result;
	}
	virtual void rebuildIndex()
	{

	}
    virtual void add(Primitive* p)
	{
		primitives.push_back(p);
		bbox.extend(p->getBounds());
	}
	virtual void setMaterial(Material* m)
	{
		for (auto i = 0; i < primitives.size(); ++i)
			primitives[i]->setMaterial(m);
	}
	virtual void setCoordMapper(CoordMapper* cm)
	{

	}
};

}

#endif
