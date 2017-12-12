#include <rt/materials/cooktorrance.h>
#include <core/color.h>
namespace rt
{
	RGBColor CookTorranceMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		// Cook-Torrance model:
		// ks = F*D*G / ((dot(n,v) * dot(n,i))
		// The fresnel term can be the one from the lectures or:
		// Simpler fresnel formula - doesn't require a kappa parameter
		// Angle in fresnel: between n and h
		// F(theta) = 1/2 * (b-c)^2 / (b+c)^2 * ( 1 + (c*(b+c)-1)^2 / (c*(b-c)+1)^2 )
		// where c = cos(theta), b^2 = eta^2 + c^2 - 1

		// The geometric term is the same as in the Torrance-Sparrow model:
		// G = min{1,  2*dot(n,h)*dot(n,v)/dot(v,h), 2*dot(n,h)*dot(n,i)/dot(v,h) }

		// Since we are required to have the Blinn distribution with the exponent from the 1977 paper:
		// Isotropic case:
		// D = powf(dot(n,h),c), c = -ln2/ln(cos(m))
		// After integrating D(theta_h)*cos(theta) we get 8*pi*(2^(-c/2)+c) / ((c+2) * (c+4))
		// where theta_h was dot(n,h) and theta was (n,i)
		// so we need to divide D by this factor for us to have a normalized isotropic Blinn distribution.
		// For the anisotropic case we have:
		// D = powf(dot(n,h),c1*(cos(phi))^2+c2*(sin(phi))^2)
		// I am assuming the normalization from Ashikhmin's paper is correct, so I have to multiply by:
		// sqrt((c1+1)*(c2+1))/(8*pi)

		// Where c1, c2 control the roughness along the principal anisotropy axes t and b (that would be the brushed direction of a metal)
		// phi is the azimuthal angle between h and t (meaning the angle between t and the projection of h in the tangent plane)
		// then (cos(phi))^2 = (dot(h,t))^2/(1-dot(n,h)^2) and (sin(phi))^2 = (dot(h,b))^2/(1-dot(n,h)^2)

		// lambert diffuse part:
		// dot(N,I)>0.0f since we check that in the integrator
		float ni = dot(normal, inDir);
		float diffuseWeight = ni / pi;

		// specular part:
		// we know that dot(outDir,normal)>0 since we flip our normal to produce this if needed
		float nv = dot(normal, outDir);
		// halfway vector between V and I
		Vector h = (inDir + outDir).normalize();
		// since ni and nv are positive, nh cannot be negative, because:
		// dot(n,h) = dot(n, (v+i)/|v+i|) = 1/|v+i| * ( dot(n,v) + dot(n,i) )
		float nh = dot(normal, h);
		// vh also cannot be negative, since dot(v,i)>=-1
		// dot(v,(v+i)/|v+i|) = 1/|v+i| * ( 1 + dot(v,i) )
		float vh = dot(outDir, h);

		// GEOMETRIC TERM:
		float geometricTerm = min(1.0f, 2.0f*nh*nv / vh, 2.0f*nh*ni / vh);

		// FRESNEL TERM:
		// this is the fresnel from the lecture:
		float fresnelTerm = fresnelReflectance(nv, eta, kappa);

		// I'll be using the one from the lecture, but down below the Cook-Torrance approximation can be seen:
		//float b = sqrtf(max(0.0f, sqr(eta) + sqr(nh) - 1.0f));
		// this cannot be negative:
		//float fresnelTerm = 0.5f * sqr(b - nh) / sqr(b + nh) * (1.0f + sqr(nh*(b + nh) - 1.0f) / sqr(nh*(b - nh) + 1.0f));

		// DISTRIBUTION TERM:

		// the interface does not permit passing a tangent vector t
		// in that case I have decided to encode a tangent that would correspond to a sphere where the brushed direction would be along phi:
		// l = sqrtf(n.x^2+n.z^2) if l = 0 -> t=(0,0,1), else (-n.z, 0.0f, n.x)/l
		float lt = sqrtf(sqr(normal.x) + sqr(normal.z));
		Vector tangent = lt!=0.0f ? Vector(-normal.z / lt, 0.0f, normal.x / lt) : Vector(0.0f, 0.0f, 1.0f);
		Vector bitangent = cross(normal, tangent);
		// cos(theta_h)^2 = (nt)^2/(1-(nh^2))
		float cfs = sqr(dot(h, tangent)) / (1.0f - sqr(nh));
		// sin(theta_h)^2 = 1 - cfs;
		float sfs = 1.0f - cfs;
		float c1 = -log(2.0f) / log(cosf(et));
		float c2 = -log(2.0f) / log(cosf(eb));
		float distributionTerm = powf(nh, cfs*c1 + sfs*c2) * sqrtf((c1+1.0f)*(c2+1.0f)) / (8.0f*pi);
		
		// pi*dot(N,V)*dot(N,I), but we have dot(N,I) in the norminator of the rendering equation so we can get rid of it
		float denom = nv;
		
		// F*D*G / (dot(n,v)*dot(n,i))
		float specularWeight = geometricTerm*fresnelTerm*distributionTerm/denom;

		return diffuseWeight*diffuse->getColor(texPoint) + specularWeight*specular->getColor(texPoint);
	}
	RGBColor CookTorranceMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return emission->getColor(texPoint);
	}
	Material::SampleReflectance CookTorranceMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		// we don't need to sample anything - just return whatever
		return SampleReflectance();
	}
}