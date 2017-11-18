#ifndef CG1RAYTRACER_SOLIDS_QUADRIC_HEADER
#define CG1RAYTRACER_SOLIDS_QUADRIC_HEADER

#include <rt/solids/solid.h>
#include <rt/intersection.h>
#include <rt/bbox.h>

namespace rt {

	class Quadric : public Solid {
	private:
		float a, b, c, d, e, f, g, h, i, j;
	public:
		Quadric() {}
		Quadric(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, CoordMapper* texMapper, Material* material)
			: a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h), i(i), j(j), Solid(texMapper, material)
		{

		}

		virtual BBox getBounds() const
		{
			return BBox();
		}
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
		{
			// this could be done with a matrix multiplication, but I still haven't implemented a 4x4 matrix class
			float A = a*ray.d.x*ray.d.x + b*ray.d.y*ray.d.y + c*ray.d.z*ray.d.z +
				d*ray.d.x*ray.d.y + e*ray.d.x*ray.d.z + f*ray.d.y*ray.d.z;
			float B = 2.0f*a*ray.o.x*ray.d.x + 2.0f*b*ray.o.y*ray.d.y + 2.0f*c*ray.o.z*ray.d.z +
				d*(ray.o.x*ray.d.y + ray.d.x*ray.o.y) + e*(ray.o.x*ray.d.z + ray.o.z*ray.d.x) + f*(ray.o.y*ray.d.z + ray.o.z*ray.d.y) +
				g*ray.d.x + h*ray.d.y + i*ray.d.z;
			float C = a*ray.o.x*ray.o.x + b*ray.o.y*ray.o.y + c*ray.o.z*ray.o.z +
				d*ray.o.x*ray.o.y + e*ray.o.x*ray.o.z + f*ray.o.y*ray.o.z +
				g*ray.o.x + h*ray.o.y + i*ray.o.z + j;

			float discriminant = B*B - 4.0f*A*C;
			if (discriminant < 0.0f) return Intersection::failure();
			float dSqrt = sqrtf(discriminant);
			float t1 = -(B+dSqrt) / (2.0f * A);
			float t2 = (-B + dSqrt) / (2.0f*A);
			if (t1 < t2)
			{
				if (t1 > previousBestDistance) return Intersection::failure();
				if (t1 > 0.0f)
				{
					Point hp = ray.getPoint(t1);
					// grad(quadric) = ( 2ax+dy+ez+g, 2by+dx+fz+h, 2cz+ex+fy+i )^T, then plugging in the intersection, we get the normal
					Vector norm(2.0f*a*hp.x + d*hp.y + e*hp.z + g, 2.0f*b*hp.y + d*hp.x + f*hp.z + h, 2.0f*c*hp.z + e*hp.x + f*hp.y + i);
					return Intersection(t1, ray, this, norm.normalize(), Point::rep(0.0f));
				}
				else
				{
					if (t2<0.0f || t2>previousBestDistance) return Intersection::failure();
					Point hp = ray.getPoint(t2);
					// grad(quadric) = ( 2ax+dy+ez+g, 2by+dx+fz+h, 2cz+ex+fy+i )^T, then plugging in the intersection, we get the normal
					Vector norm(2.0f*a*hp.x + d*hp.y + e*hp.z + g, 2.0f*b*hp.y + d*hp.x + f*hp.z + h, 2.0f*c*hp.z + e*hp.x + f*hp.y + i);
					return Intersection(t2, ray, this, norm.normalize(), Point::rep(0.0f));
				}
			}
			else
			{
				if (t2 > previousBestDistance) return Intersection::failure();
				if (t2 > 0.0f)
				{
					Point hp = ray.getPoint(t2);
					// grad(quadric) = ( 2ax+dy+ez+g, 2by+dx+fz+h, 2cz+ex+fy+i )^T, then plugging in the intersection, we get the normal
					Vector norm(2.0f*a*hp.x + d*hp.y + e*hp.z + g, 2.0f*b*hp.y + d*hp.x + f*hp.z + h, 2.0f*c*hp.z + e*hp.x + f*hp.y + i);
					return Intersection(t2, ray, this, norm.normalize(), Point::rep(0.0f));
				}
				else
				{
					if (t1<0.0f || t1>previousBestDistance) return Intersection::failure();
					Point hp = ray.getPoint(t1);
					// grad(quadric) = ( 2ax+dy+ez+g, 2by+dx+fz+h, 2cz+ex+fy+i )^T, then plugging in the intersection, we get the normal
					Vector norm(2.0f*a*hp.x + d*hp.y + e*hp.z + g, 2.0f*b*hp.y + d*hp.x + f*hp.z + h, 2.0f*c*hp.z + e*hp.x + f*hp.y + i);
					return Intersection(t1, ray, this, norm.normalize(), Point::rep(0.0f));
				}
			}
		}
		virtual Point sample() const
		{
			return Point::rep(0.0f);
		}
		virtual float getArea() const
		{
			return 0.0f;
		}
	};

}


#endif
