#include <core/julia.h>
#include <core/point.h>
#include <core/vector.h>

namespace rt
{
	int julia(const Point& v, const Point& c)
	{
		Vector p = v - Point::rep(0.0f);
		int i = 0;
		for (i = 0; i<512; ++i) {
			if (p.lensqr()>1.0e+8)
				break;
			Vector q = p;
			q.y = -p.y;
			p = Vector(dot(q, p), cross(q, p).z, 0.0f) + c - Point::rep(0.0f);
		}
		return i;
	}
}