#ifndef CG1RAYTRACER_SOLIDS_BOX_HEADER
#define CG1RAYTRACER_SOLIDS_BOX_HEADER

#include <rt/solids/solid.h>
#include <rt/bbox.h>
#include <rt/intersection.h>

namespace rt {

class AABox : public Solid {
private:
	Point min;
	Point max;
public:
    AABox() {}
	AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
		: Solid(texMapper, material)
	{
		min = rt::min(corner1, corner2);
		max = rt::max(corner1, corner2);
	}

	virtual BBox getBounds() const
	{
		return BBox(min, max);
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		float tmin = 0.0f;
		float tmax = previousBestDistance;
		Vector norm;
		// yz planes:
		// ray.o.x + ray.d.x*t = min.x && ray.o.x + ray.d.x*t = max.x
		//  if d is parallel to the yz plane
		if (-epsilon < ray.d.x && ray.d.x < epsilon)
		{
			if (min.x > ray.o.x || max.x < ray.o.x) return Intersection::failure();
		}
		else
		{
			float invDx = 1.0f / ray.d.x;
			float t1 = (min.x - ray.o.x) * invDx;
			float t2 = (max.x - ray.o.x) * invDx;
			if (t1 > t2)
			{
				if (tmin < t2)
				{
					tmin = t2;
					norm = Vector(1.0f, 0.0f, 0.0f);
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
					norm = Vector(-1.0f, 0.0f, 0.0f);
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
			if (tmax < tmin) return Intersection::failure();
		}

		// xz planes:
		// if d is parallel to the yz planes
		if (-epsilon < ray.d.y && ray.d.y < epsilon)
		{
			if (min.y > ray.o.y || max.y < ray.o.y) return Intersection::failure();
		}
		else
		{
			float invDy = 1.0f / ray.d.y;
			float t1 = (min.y - ray.o.y) * invDy;
			float t2 = (max.y - ray.o.y) * invDy;
			if (t1 > t2)
			{
				if (tmin < t2)
				{
					tmin = t2;
					norm = Vector(0.0f, 1.0f, 0.0f);
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
					norm = Vector(0.0f, -1.0f, 0.0f);
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
			if (tmax < tmin) return Intersection::failure();
		}

		// xz planes:
		// if d is parallel to the yz planes
		if (-epsilon < ray.d.z && ray.d.z < epsilon)
		{
			if (min.z > ray.o.z || max.z < ray.o.z) return Intersection::failure();
		}
		else
		{
			float invDz = 1.0f / ray.d.z;
			float t1 = (min.z - ray.o.z) * invDz;
			float t2 = (max.z - ray.o.z) * invDz;
			if (t1 > t2)
			{
				if (tmin < t2)
				{
					tmin = t2;
					norm = Vector(0.0f, 0.0f, 1.0f);
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
					norm = Vector(0.0f, 0.0f, -1.0f);
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
			if (tmax < tmin) return Intersection::failure();
		}
		return Intersection(tmin, ray, this, norm, Point::rep(0.0f));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		Vector d = max - min;
		return 2.0f*(d.x*d.y + d.x*d.z + d.y*d.z);
	}
};

}

#endif
