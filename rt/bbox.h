#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <rt/ray.h>

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;
	bool unbound;

    BBox() : min(Point::rep(1.0f)), max(Point::rep(-1.0f)) {}
    BBox(const Point& min, const Point& max) : min(min), max(max), unbound(false) {}
	static BBox empty()
	{
		return BBox(Point::rep(1.0f), Point::rep(-1.0f));
	}
	static BBox full()
	{
		BBox result(Point::rep(-FLT_MAX), Point::rep(FLT_MAX));
		result.unbound = true;
		return result;
	}

	void extend(const Point& point)
	{
		max = rt::max(max, point);
		min = rt::min(min, point);
	}
	void extend(const BBox& bbox)
	{
		max = rt::max(max, bbox.max);
		min = rt::min(min, bbox.min);
	}

	bool isEmpty() const
	{
		return min.x >= max.x || min.y >= max.y || min.z >= max.z;
	}

	bool overlap(const BBox& bbox) const
	{
		return !BBox(rt::max(min, bbox.min), rt::min(max, bbox.max)).isEmpty();
	}

    Vector diagonal() const { return max - min; }



	std::pair<float, float> intersect(const Ray& ray) const
	{
		float tmin = -FLT_MAX;
		float tmax = FLT_MAX;
		// yz planes:
		// ray.o.x + ray.d.x*t = min.x && ray.o.x + ray.d.x*t = max.x
		//  if d is parallel to the yz plane
		if (-epsilon < ray.d.x && ray.d.x < epsilon)
		{
			if (min.x > ray.o.x || max.x < ray.o.x) return std::make_pair(FLT_MAX, -FLT_MAX);
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
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
			if (tmax < tmin) return std::make_pair(tmin, tmax);
		}

		// xz planes:
		// if d is parallel to the yz planes
		if (-epsilon < ray.d.y && ray.d.y < epsilon)
		{
			if (min.y > ray.o.y || max.y < ray.o.y) return std::make_pair(FLT_MAX, -FLT_MAX);
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
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
			if (tmax < tmin) return std::make_pair(tmin, tmax);
		}

		// xz planes:
		// if d is parallel to the yz planes
		if (-epsilon < ray.d.z && ray.d.z < epsilon)
		{
			if (min.z > ray.o.z || max.z < ray.o.z) return std::make_pair(FLT_MAX, -FLT_MAX);
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
				}
				tmax = tmax > t1 ? t1 : tmax;
			}
			else
			{
				if (tmin < t1)
				{
					tmin = t1;
				}
				tmax = tmax > t2 ? t2 : tmax;
			}
		}
		return std::make_pair(tmin, tmax);
	}

	bool isUnbound()
	{
		return unbound;
	}
};

}


#endif
