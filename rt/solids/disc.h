#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>

namespace rt {

class Disc : public Solid {
private:
	Point center;
	float radius;
	Vector normal;
public:
    Disc() {}
	Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
		: center(center), normal(normal.normalize()), radius(radius), Solid(texMapper, material)
	{

	}

	virtual BBox getBounds() const
	{
		// To find the min and max on x, y, z axis, we take the gradient of p:[0,2pi]^3->R^3
		// p(a) = center + ux*radius*cos(a) + uy*radius*sin(a): grad(p(a)) = 0 => we solve for each component of a
		// and then we just find the min and max for each component from p(a) and p(a+pi):
		// Note: normal, uxand uy form an orthonormal basis

		// cx + ux*c(a) + vx*s(a) = px(a) => -ux*s(a) + vx*c(a) = 0 =>\
		// ux/vx * tg(a) = 1 => tg(a) = vx/ux => a = atan(vx/ux), minx = min(px(a),px(a+pi)), maxx = max(px(a), px(a+pi))
		const Vector localX = perp(normal).normalize();
		const Vector localY = cross(normal, localX);
		Vector mn, mx;
		// min and max x
		if (localX.x == 0.0f && localY.x == 0.0f)
		{
			mn.x = -epsilon;
			mx.x = epsilon;
		}
		else if (localX.x == 0.0f && localY.x != 0.0f)
		{
			//cos(pi/2)=0 => sin(pi/2) = 1 && sin(3/2pi) = -1
			float v = radius*localY.x;
			mn.x = min(-v, v);
			mx.x = max(-v, v);
		}
		else if (localX.x != 0.0f && localY.x == 0.0f)
		{
			//sin(0) = 0 => cos(0) = 1 && cos(pi) = -1
			float v = radius*localX.x;
			mn.x = min(-v,v);
			mx.x = max(-v,v);
		}
		else
		{
			//cos(atan(q)) = 1/sqrtf(1+q*q)
			float q = localX.x / localY.x;
			//sin(atan(q)) = x/sqrtf(1+q*q)
			float sq = 1.0f / sqrtf(1.0f + q*q);
			float c = sq;
			float s = q*sq;
			float v = localX.x*c + localY.x*s;
			//cos(a+pi) = -cos(a), sin(a+pi) = -sin(a) => if p(a) = v => p(a+pi) = -v
			mn.x = min(-v, v);
			mx.x = max(-v, v);
		}

		// min and max y
		if (localX.y == 0.0f && localY.y == 0.0f)
		{
			mn.y = -epsilon;
			mx.y = epsilon;
		}
		else if (localX.y == 0.0f && localY.y != 0.0f)
		{
			//cos(pi/2)=0 => sin(pi/2) = 1 && sin(3/2pi) = -1
			float v = radius*localY.y;
			mn.y = min(-v, v);
			mx.y = max(-v, v);
		}
		else if (localX.y != 0.0f && localY.y == 0.0f)
		{
			//sin(0) = 0 => cos(0) = 1 && cos(pi) = -1
			float v = radius*localX.y;
			mn.y = min(-v, v);
			mx.y = max(-v, v);
		}
		else
		{
			//cos(atan(q)) = 1/sqrtf(1+q*q)
			float q = localX.y / localY.y;
			//sin(atan(q)) = x/sqrtf(1+q*q)
			float sq = 1.0f / sqrtf(1.0f + q*q);
			float c = sq;
			float s = q*sq;
			float v = radius*(localX.y*c + localY.y*s);
			//cos(a+pi) = -cos(a), sin(a+pi) = -sin(a) => if p(a) = v => p(a+pi) = -v
			mn.y = min(-v, v);
			mx.y = max(-v, v);
		}

		// min and max z
		if (localX.z == 0.0f && localY.z == 0.0f)
		{
			mn.z = -epsilon;
			mx.z = epsilon;
		}
		else if (localX.z == 0.0f && localY.z != 0.0f)
		{
			//cos(pi/2)=0 => sin(pi/2) = 1 && sin(3/2pi) = -1
			float v = radius*localY.z;
			mn.z = min(-v, v);
			mx.z = max(-v, v);
		}
		else if (localX.z != 0.0f && localY.z == 0.0f)
		{
			//sin(0) = 0 => cos(0) = 1 && cos(pi) = -1
			float v = radius*localX.z;
			mn.z = min(-v, v);
			mx.z = max(-v, v);
		}
		else
		{
			//cos(atan(q)) = 1/sqrtf(1+q*q)
			float q = localX.z / localY.z;
			//sin(atan(q)) = x/sqrtf(1+q*q)
			float sq = 1.0f / sqrtf(1.0f + q*q);
			float c = sq;
			float s = q*sq;
			float v = radius*(localX.z*c + localY.z*s);
			//cos(a+pi) = -cos(a), sin(a+pi) = -sin(a) => if p(a) = v => p(a+pi) = -v
			mn.z = min(-v, v);
			mx.z = max(-v, v);
		}

		return BBox(center + mn, center + mx);
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		float dt = dot(ray.d, normal);
		// is the ray parallel to the plane - if it is just return no intersection
		// even if the ray lies exactly in the plane, I'd rather return no intersection
		//if (dt > -epsilon || dt < epsilon) return Intersection::failure();
		float t = dot(normal, center - ray.o) / dt;
		// if the intersection is not in the ray's direction or there's a closer intersection - ignore it - no intersection
		if (t < 0.0f || t>previousBestDistance) return Intersection::failure();
		// if the intersection is outside of the disk - ignore it - no intersection
		Vector dst = ray.getPoint(t) - center;
		if (dot(dst, dst) > radius*radius) return Intersection::failure();
		return Intersection(t, ray, this, normal, Point::rep(0.0f));
	}
	virtual Point sample() const
	{
		return Point::rep(0.0f);
	}
	virtual float getArea() const
	{
		return pi*radius*radius;
	}
};

}


#endif
