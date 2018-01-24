#ifndef VCDR_PROCEDURAL_HEADER
#define VCDR_PROCEDURAL_HEADER
#include <rendering_comp/vcdr_util.h>
#include <rendering_comp/vcdr_vec.h>
#include <rendering_comp/vcdr_mat.h>
namespace rt
{
	///////////////////////////////////////////////////////////////////////////////////////
	//                                     HASHES                                        //
	///////////////////////////////////////////////////////////////////////////////////////

	struct hash12_iq0
	{
		static float hash(vec2 v)
		{
			// Author Inigo Quilez:
			// doesn't seem to be good around 0
			vec2 p = 50.0f*fract(v*0.3183099f);
			return fract(p.x*p.y*(p.x + p.y));
		}
	};

	struct hash11_iq0
	{
		static float hash(float v)
		{
			// Author Inigo Quilez:
			return fract(v*17.0f*fract(v*0.3183099f));
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////
	//                                     VALUE NOISE                                   //
	///////////////////////////////////////////////////////////////////////////////////////

	template <class hash12 = hash12_iq0>
	struct ValueNoise2D
	{
		template<bool remap=false>
		static float noise(vec2 p)
		{
			vec2 wholePart = floor(p);
			vec2 fractPart = p - wholePart;

			// quintic smoothstep: 6*x^5 - 15*x^4 + 10*x^3, has 1st and 2nd order derviative equal to 0 at x=0,x=1
			// first suggested by Ken Perlin: https://en.wikipedia.org/wiki/Smoothstep
			vec2 s = fractPart*fractPart*fractPart*(fractPart*(fractPart*6.0f - 15.0f) + 10.0f);

			// the 4 corners
			float a00 = hash12::hash(wholePart);
			float a10 = hash12::hash(wholePart + vec2(1.0f, 0.0f));
			float a01 = hash12::hash(wholePart + vec2(0.0f, 1.0f));
			float a11 = hash12::hash(wholePart + vec2(1.0f, 1.0f));

			// "bilinear" interpolation (using sx and sy rather than fractPartX and fractPartY to make the interpolation smoother):
			// (1-sy)*(a00*(1-sx)+a10*sx) + sy*(a01*(1-sx)+a11*sx) = a00 + (a10 - a00)*sx + (a01 - a00)*sy + (a00 - a10 - a01 + a11)*sx*sy
			float value = a00 + (a10 - a00)*s.x + (a01 - a00)*s.y + (a00 - a10 - a01 + a11)*s.x*s.y;
			return value;
		}

		template<>
		static float noise<true>(vec2 p)
		{
			return 2.0f*noise(p) - 1.0f;
		}

		template<bool remap=false>
		static vec3 noiseD(vec2 p)
		{
			vec2 wholePart = floor(p);
			vec2 fractPart = p - wholePart;

			// quintic smoothstep: 6*x^5 - 15*x^4 + 10*x^3, has 1st and 2nd order derviative equal to 0 at x=0,x=1
			// first suggested by Ken Perlin: https://en.wikipedia.org/wiki/Smoothstep
			vec2 s = fractPart*fractPart*fractPart*(fractPart*(fractPart*6.0f - 15.0f) + 10.0f);

			// the 4 corners
			float a00 = hash12::hash(wholePart);
			float a10 = hash12::hash(wholePart + vec2(1.0f, 0.0f));
			float a01 = hash12::hash(wholePart + vec2(0.0f, 1.0f));
			float a11 = hash12::hash(wholePart + vec2(1.0f, 1.0f));

			// the 4 coefficients for the billinear interpolatgion
			float k0 = a00;
			float k1 = a10 - a00;
			float k2 = a01 - a00;
			float k3 = a00 - a10 - a01 + a11;
			// "bilinear" interpolation (using sx and sy rather than fractPartX and fractPartY to make the interpolation smoother):
			// (1-sy)*(a00*(1-sx)+a10*sx) + sy*(a01*(1-sx)+a11*sx) = a00 + (a10 - a00)*sx + (a01 - a00)*sy + (a00 - a10 - a01 + a11)*sx*sy
			float value = k0 + k1*s.x + k2*s.y + k3*s.x*s.y;
			// d(6*x^5 - 15*x^4 + 10*x^3)/dx = 30*x^4 - 60*x^3 + 30*x^2 = 30*x^2*(x*(x-2)+1)
			vec2 ds = 30.0f*fractPart*fractPart*(fractPart*(fractPart - 2.0f) + 1.0f);
			return vec3(value, (k1 + k3*s.y)*ds.x, (k2 + k3*s.x)*ds.y);
		}

		template<>
		static vec3 noiseD<true>(vec2 p)
		{
			vec3 result = 2.0f*noiseeD(p);
			result.x -= 1.0f;
			return result;
		}

	};

	template <class hash11 = hash11_iq0>
	struct ValueNoise3D
	{
		template<bool remap=false>
		static float noise(vec3 p)
		{
			vec3 wholePart = floor(p);
			vec3 fractPart = p - wholePart;

			// quintic smoothstep: 6*x^5 - 15*x^4 + 10*x^3, has 1st and 2nd derviative equal 0 at x=0,x=1
			// first suggested by Ken Perlin: https://en.wikipedia.org/wiki/Smoothstep
			vec3 s = fractPart*fractPart*fractPart*(fractPart*(fractPart*6.0f - 15.0f) + 10.0f);

			// encode the 3 coordinates in 1 through rescaling (non-optimal, but I didn't find anything better that did not rely on textures)
			float n = wholePart.x + 317.0f*wholePart.y + 157.0f*wholePart.z;

			// the 8 corners
			float a000 = hash11::hash(n);
			float a100 = hash11::hash(n + 1.0f);
			float a010 = hash11::hash(n + 317.0f);
			float a110 = hash11::hash(n + 318.0f);
			float a001 = hash11::hash(n + 157.0f);
			float a101 = hash11::hash(n + 158.0f);
			float a011 = hash11::hash(n + 474.0f);
			float a111 = hash11::hash(n + 475.0f);

			// "trilinear" interpolation:
			// (1-sz)*((1-sy)*(a000*(1-sx)+a100*sx) + sy*(a010*(1-sx)+a110*sx)) + sz*((1-sy)*(a001*(1-sx)+a101*sx) + sy*(a011*(1-sx)+a111*sx)) =
			// = (1-sz)*(a000 + (a100 - a000)*sx + (a010 - a000)*sy + (a000 - a100 - a010 + a110)*sx*sy) + 
			// + sz*(a001 + (a101 - a001)*sx + (a011 - a001)*sy + (a001 - a101 - a011 + a111)*sx*sy) =
			// = a000 + (a100 - a000)*sx + (a010 - a000)*sy + (a001 - a000)*sz + 
			// + (a000 - a100 - a010 + a110)*sx*sy + (a000 - a010 - a001 + a011)*sy*sz + (a000 - a100 - a001 + a101)*sx*sz +
			// + (-a000 + a100 + a010 - a110 + a001 - a101 - a011 + a111)*sx*sy*sz 
			float k0 = a000;
			float k1 = a100 - a000;
			float k2 = a010 - a000;
			float k3 = a001 - a000;
			float k4 = a000 - a100 - a010 + a110;
			float k5 = a000 - a010 - a001 + a011;
			float k6 = a000 - a100 - a001 + a101;
			float k7 = -a000 + a100 + a010 - a110 + a001 - a101 - a011 + a111;

			float value = k0 + k1*s.x + k2*s.y + k3*s.z + k4*s.x*s.y + k5*s.y*s.z + k6*s.x*s.z + k7*s.x*s.y*s.z;
			return value;
		}

		template<>
		static float noise<true>(vec3 p)
		{
			return 2.0f*noise(p)-1.0f;
		}

		template<bool remap=false>
		static vec4 noiseD(vec3 p)
		{
			vec3 wholePart = floor(p);
			vec3 fractPart = p - wholePart;

			// quintic smoothstep: 6*x^5 - 15*x^4 + 10*x^3, has 1st and 2nd derviative equal 0 at x=0,x=1
			// first suggested by Ken Perlin: https://en.wikipedia.org/wiki/Smoothstep
			vec3 s = fractPart*fractPart*fractPart*(fractPart*(fractPart*6.0f - 15.0f) + 10.0f);

			float n = wholePart.x + 317.0f*wholePart.y + 157.0f*wholePart.z;

			// the 8 corners
			float a000 = hash11::hash(n);
			float a100 = hash11::hash(n + 1.0f);
			float a010 = hash11::hash(n + 317.0f);
			float a110 = hash11::hash(n + 318.0f);
			float a001 = hash11::hash(n + 157.0f);
			float a101 = hash11::hash(n + 158.0f);
			float a011 = hash11::hash(n + 474.0f);
			float a111 = hash11::hash(n + 475.0f);

			// "trilinear" interpolation:
			// (1-sz)*((1-sy)*(a000*(1-sx)+a100*sx) + sy*(a010*(1-sx)+a110*sx)) + sz*((1-sy)*(a001*(1-sx)+a101*sx) + sy*(a011*(1-sx)+a111*sx)) =
			// = (1-sz)*(a000 + (a100 - a000)*sx + (a010 - a000)*sy + (a000 - a100 - a010 + a110)*sx*sy) + 
			// + sz*(a001 + (a101 - a001)*sx + (a011 - a001)*sy + (a001 - a101 - a011 + a111)*sx*sy) =
			// = a000 + (a100 - a000)*sx + (a010 - a000)*sy + (a001 - a000)*sz + 
			// + (a000 - a100 - a010 + a110)*sx*sy + (a000 - a010 - a001 + a011)*sy*sz + (a000 - a100 - a001 + a101)*sx*sz +
			// + (-a000 + a100 + a010 - a110 + a001 - a101 - a011 + a111)*sx*sy*sz 
			float k0 = a000;
			float k1 = a100 - a000;
			float k2 = a010 - a000;
			float k3 = a001 - a000;
			float k4 = a000 - a100 - a010 + a110;
			float k5 = a000 - a010 - a001 + a011;
			float k6 = a000 - a100 - a001 + a101;
			float k7 = -a000 + a100 + a010 - a110 + a001 - a101 - a011 + a111;

			float value = k0 + k1*s.x + k2*s.y + k3*s.z + k4*s.x*s.y + k5*s.y*s.z + k6*s.x*s.z + k7*s.x*s.y*s.z;
			// d(6*x^5 - 15*x^4 + 10*x^3)/dx = 30*x^4 - 60*x^3 + 30*x^2 = 30*x^2*(x*(x-2)+1)
			vec3 ds = 30.0f*fractPart*fractPart*(fractPart*(fractPart - 2.0f) + 1.0f);
			float dxv = (k1 + k4*s.y + k6*s.z + k7*s.y*s.z)*ds.x;
			float dyv = (k2 + k4*s.x + k5*s.x + k7*s.x*s.z)*ds.y;
			float dzv = (k3 + k5*s.y + k6*s.z + k7*s.x*s.y)*ds.z;
			return vec4(value, dxv, dyv, dzv);
		}

		template<>
		static vec4 noiseD<true>(vec3 p)
		{
			vec4 result = 2.0f*noiseD(p);
			result.x -= 1.0f;
			return result;
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////
	//                               FRACTAL BROWNIAN MOTION                             //
	///////////////////////////////////////////////////////////////////////////////////////

	

	template <class noise2d = ValueNoise2D<hash12_iq0> >
	struct FBM2D
	{
		// 2d rotation matrices for axial bias mitigation
		static const mat2 FBM2DRMat;
		static const mat2 FBM2DRMatT;
		// fbms without axial bias mitigation
		template<bool remap=false>
		static float fbmc(vec2 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the amplitude power here:
			float amplitude = 1.0f;
			// and we'll accumulate the frequency directly in p
			for (int i = 0; i < layers; ++i)
			{
				value += amplitude * noise2d::noise<remap>(p);
				amplitude *= AM;
				p *= FM;
			}

			return value;
		}

		// fbms with axial bias mitigation
		template<bool remap=false>
		static float fbm(vec2 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the amplitude power here:
			float amplitude = 1.0f;
			for (int i = 0; i < layers; ++i)
			{
				value += amplitude * noise2d::noise<remap>(p);
				amplitude *= AM;
				// the points are on a spiral
				p = FBM2DRMat*p*FM;
			}

			return value;
		}

		// 2d fbm with derivatives
		// Derivatives derivation:
		// the i-th term in the sum looks like this: amplitudeMultiplier^i * valueNoise(M^i * (x,y)^T * frequencyMultiplier)
		// let M^i = (m11, m12, m21, m22)
		// taking the derivative with respect to x: amplitudeMultiplier^i * frequencyMultiplier * 
		// ( d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dx * m11 + d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dy * m21)
		// taking the derivative with respect to y: amplitudeMultiplier^i * frequencyMultiplier * 
		// ( d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dx * m12 + d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dy * m22)
		// => grad(amplitudeMultiplier^i * valueNoise(M^i * (x,y)^T * frequencyMultiplier)) = 
		// = amplitudeMultiplier^i * frequencyMultiplier * (M^i)^T * (d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dx, d(valueNoise(M^i * (x,y)^T * frequencyMultiplier))/dy)^T
		template<bool remap = false>
		static vec3 fbmD(vec2 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the derivatives here:
			vec2 d = vec2(0.0f);
			// accumulate the amplitude power here:
			float amplitude = 1.0f;

			// we accumulate (M^i)^T here
			mat2 m = mat2::identity();
			for (int i = 0; i < layers; ++i)
			{
				vec3 vn = noise2d::noiseD<remap>(p);
				value += amplitude * vn.x;
				d += m * vn.yz() * amplitude;
				amplitude *= AM;
				// the points are on a spiral
				p = FBM2DRMat * p * FM;

				// multiply m with the transpose of the rotation matrix at each iteration:
				// M = frequencyMultiplier * R^T * M
				m = FM*FBM2DRMatT*m;
			}

			return vec3(value, d);
		}
	};
	template <class noise2d>
	const mat2 FBM2D<noise2d>::FBM2DRMat = mat2(0.8f, 0.6f, -0.6f, 0.8f);
	template <class noise2d>
	const mat2 FBM2D<noise2d>::FBM2DRMatT = mat2(0.8f, -0.6f, 0.6f, 0.8f);

	template <class noise3d = ValueNoise3D<hash11_iq0> >
	struct FBM3D
	{
		// 2d rotation matrices for axial bias mitigation
		static const mat3 FBM3DRMat;
		static const mat3 FBM3DRMatT;
		// fbms without axial bias mitigation
		template<bool remap = false>
		static float fbmc(vec3 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the amplitude power here:
			float amplitude = 1.0f;
			// and we'll accumulate the frequency directly in p
			for (int i = 0; i < layers; ++i)
			{
				value += amplitude * noise3d::noise<remap>(p);
				amplitude *= AM;
				p *= FM;
			}

			return value;
		}

		// fbms with axial bias mitigation
		template<bool remap = false>
		static float fbm(vec3 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the amplitude power here:
			float amplitude = 1.0f;
			for (int i = 0; i < layers; ++i)
			{
				value += amplitude * noise3d::noise<remap>(p);
				amplitude *= AM;
				// the points are on a spiral
				p = FBM3DRMat*p*FM;
			}

			return value;
		}

		template<bool remap = false>
		static vec4 fbmD(vec3 p, int layers = 4, float AM = 0.5f, float FM = 2.0f)
		{
			// accumulate the result here:
			float value = 0.0f;
			// accumulate the derivatives here:
			vec3 d = vec3(0.0f);
			// accumulate the amplitude power here:
			float amplitude = 1.0f;

			// we accumulate (M^i)^T here
			mat3 m = mat3::identity();
			for (int i = 0; i < layers; ++i)
			{
				vec4 vn = noise3d::noiseD<remap>(p);
				value += amplitude * vn.x;
				d += m * vn.yzw() * amplitude;
				amplitude *= AM;
				// the points are on a spiral
				p = FBM3DRMat * p * FM;

				// multiply m with the transpose of the rotation matrix at each iteration:
				// M = frequencyMultiplier * R^T * M
				m = FM*FBM3DRMatT*m;
			}

			return vec4(value, d);
		}
	};
	template <class noise3d>
	const mat3 FBM3D<noise3d>::FBM3DRMat = mat3(0.0f, 0.8f, 0.6f, -0.8f, 0.36f, -0.48f, -0.6f, -0.48f, 0.64f);
	template <class noise3d>
	const mat3 FBM3D<noise3d>::FBM3DRMatT = mat3(0.0f, -0.8f, -0.6f, 0.8f, 0.36f, -0.48f, 0.6f, -0.48f, 0.64f);
}

#endif

