#include <rt/materials/material.h>

namespace rt
{
	float fresnelReflectance(float cosA, float eta, float kappa)
	{
		// max in case of precision error
		float c = sqrtf(max(0.0f, 1.0f - cosA*cosA));
		float cs = c*c;
		float ek = eta*eta + kappa*kappa;
		float rParallelSq = (ek*cs - 2.0f*eta*c + 1.0f) / (ek*cs + 2.0f*eta*c + 1.0f);
		float rPerpSq = (ek - 2.0f*eta*c + cs) / (ek + 2.0f*eta*c + cs);
		float refl = 0.5f*(rParallelSq + rPerpSq);
		return refl;
	}
}