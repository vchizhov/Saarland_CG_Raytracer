#include <rt/primmod/bmap.h>
#include <rt/solids/triangle.h>
#include <rt/textures/texture.h>
#include <core/color.h>

namespace rt
{
	BumpMapper::BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale)
		: triangle(base), bumpmap(bumpmap),  bv1(bv1), bv2(bv2), bv3(bv3), vscale(vscale)
	{

	}
	BBox BumpMapper::getBounds() const
	{
		return triangle->getBounds();
	}
	Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const
	{
		Intersection result = triangle->intersect(ray, previousBestDistance);
		// linear combination of the uv coordinates
		Point uv = Point::rep(0.0f) + result.local().x*(bv1 - Point::rep(0.0f)) + result.local().y*(bv2 - Point::rep(0.0f)) + result.local().z*(bv3 - Point::rep(0.0f));
		RGBColor dx = bumpmap->getColorDX(uv);
		RGBColor dy = bumpmap->getColorDY(uv);
		// transform e1 and e2 to world space
		// we'll transform (0,0), (1,0), (0,1) to achieve that:
		// we have alpha*bv1+beta*bv2+(1-alpha-beta)*bv3 = (u,v,*) =>
		// alpha*(bv1-bv3) + beta*(bv2-bv3) = (u,v,*)-bv3 -> solve the system
		// | A*alpha+B*beta = C, A = (bv1-bv3).x, B = (bv2-bv3).x, C = u-bv3.x
		// | D*alpha+E*beta = F, D = (bv1-bv3).y, E = (bv2-bv3).y, C = v-bv3.y
		float A, B, C, D, E, F;
		float u, v;
		float det, detA, detB;
		float alpha1, beta1;
		A = bv1.x - bv3.x; B = bv2.x - bv3.x;
		D = bv1.y - bv3.y; E = bv2.y - bv3.y;

		// calculate for (u,v) = (0,0)
		u = 0.0f;
		v = 0.0f;
		C = u - bv3.x;
		F = v - bv3.y;
		// Cramer
		det = A*E - B*D;
		detA = C*E - F*B;
		detB = A*F - C*D;
		alpha1 = detA / det;
		beta1 = detB / det;
		Vector t00 = alpha1*(triangle->v1 - Point::rep(0.0f)) + beta1*(triangle->v2 - Point::rep(0.0f)) + (1.0f - alpha1 - beta1)*(triangle->v3 - Point::rep(0.0f));

		// calculate for (u,v) = (1,0)
		u = 1.0f;
		v = 0.0f;
		C = u - bv3.x;
		F = v - bv3.y;
		// Cramer
		detA = C*E - F*B;
		detB = A*F - C*D;
		alpha1 = detA / det;
		beta1 = detB / det;
		Vector t10 = alpha1*(triangle->v1 - Point::rep(0.0f)) + beta1*(triangle->v2 - Point::rep(0.0f)) + (1.0f - alpha1 - beta1)*(triangle->v3 - Point::rep(0.0f));

		// calculate for (u,v) = (0,1)
		u = 0.0f;
		v = 1.0f;
		C = u - bv3.x;
		F = v - bv3.y;
		// Cramer
		detA = C*E - F*B;
		detB = A*F - C*D;
		alpha1 = detA / det;
		beta1 = detB / det;
		Vector t01 = alpha1*(triangle->v1 - Point::rep(0.0f)) + beta1*(triangle->v2 - Point::rep(0.0f)) + (1.0f - alpha1 - beta1)*(triangle->v3 - Point::rep(0.0f));

		// e1 and e2 in 3d
		// for some reason the reference image has flipped wx and wy
		Vector wx = -(t10 - t00).normalize();
		Vector wy = -(t01 - t00).normalize();
		Vector dxW(dx.r*wx.x, dx.g*wx.y, dx.b*wx.z);
		Vector dyW(dy.r*wy.x, dy.g*wy.y, dy.b*wy.z);
		Vector normal = (result.normal() + dx.r*cross(result.normal(), wy) - dy.r*cross(result.normal(), wx)).normalize();
		result.norm = normal;
		return result;
	}
	void BumpMapper::setMaterial(Material* m)
	{
		material = m;
	}
	void BumpMapper::setCoordMapper(CoordMapper* cm)
	{
		texMapper = cm;
	}
}