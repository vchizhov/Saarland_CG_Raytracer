#ifndef CG1RAYTRACER_PRIMMOD_INSTANCE_HEADER
#define CG1RAYTRACER_PRIMMOD_INSTANCE_HEADER

#include <rt/primitive.h>
#include <core/matrix.h>
#include <iostream>
namespace rt {

class Instance : public Primitive {
private:
	Primitive* archetype;
	Matrix mat;
	Matrix matInv;
	BBox bbox;

	void updateBBox()
	{
		BBox bb = archetype->getBounds();
		bb.min = mat*bb.min;
		bb.max = mat*bb.max;
		Vector d = bb.diagonal()/2;
		Point center = bb.min + d;
		float lg = d.length();
		bbox.min = center - Vector::rep(lg);
		bbox.max = center + Vector::rep(lg);
	}
public:

	Instance(Primitive* content) : archetype(content), mat(Matrix::identity()), matInv(Matrix::identity()), bbox(content->getBounds()) {};
	Primitive* content() { return archetype; }

	void reset() //reset transformation back to identity
	{
		mat  = Matrix::identity();
		matInv = Matrix::identity();
		bbox = archetype->getBounds();
	}
	void translate(const Vector& t)
	{
		mat = product(Matrix::translation(t), mat);
		matInv = product(matInv, Matrix::translation(-t));
		updateBBox();
	}
	void rotate(const Vector& axis, float angle)
	{
		Vector normalizedAxis = axis.normalize();
		mat = product(Matrix::rotation(normalizedAxis, angle), mat);
		matInv = product(matInv, Matrix::rotation(normalizedAxis, -angle));
		updateBBox();
	}
	void scale(float scale)
	{
		mat = scale*mat;
		matInv = mat * (1.0f / scale);
		updateBBox();
	}
	void scale(const Vector& scale)
	{
		mat = product(Matrix::scaling(scale), mat);
		matInv = product(matInv, Matrix::scaling(Vector(1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z)));
		updateBBox();
	}

	void print(std::ostream& os = std::cout) const
	{
		os << "Matrix:" << mat.transpose();
		os << "Inverse Matrix: " << matInv.transpose();
	}

	// is wrong
	virtual BBox getBounds() const
	{
		return bbox;
	}
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
	{
		// I want to keep the ray.d normalized
		// so if there is scaling, I would just record its length and normalize it
		// it can be shown that if I divide previousBestDistance with its length, and then
		// divide the resulting distance with the length too, I should get the same result
		// as when scaling the vector and not normalizing it
		Vector rd = matInv*ray.d;
		float lgtInv = 1.0f/rd.length();
		rd = rd * lgtInv;
		Intersection result = archetype->intersect(Ray(matInv*ray.o, rd), previousBestDistance*lgtInv);
		if (!result) return Intersection::failure();
		// normal transform: (M^-1)^T
		result.norm = (matInv.transpose()*result.norm).normalize();
		result.ray = ray;
		result.distance *= lgtInv;
		return result;
	}
	virtual void setMaterial(Material* m)
	{

	}
	virtual void setCoordMapper(CoordMapper* cm)
	{

	}

};

}

#endif