#include <Material.h>

#include <World.h>

namespace RE
{
	Matte::Matte(F32 ka, F32 kd, const Color& cd) : ambientBRDF(ka, cd), diffuseBRDF(kd, cd)
	{
	}

	Matte::~Matte()
	{

	}

	Color Matte::Shade(const ElementIntersection& ei, const World& world)
	{
		VML::Vector wo = ei.ray.GetDirection().negate();

		// Add ambient light
		Color color = ambientBRDF.Rho(ei, wo) * world.ambientLight.CalculateRadiance(ei);

		// Add color from each light
		auto lights = world.GetLights();
		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			Light * light = *it;
			VML::Vector wi = light->GetDirectionFromPoint(ei.rayInt.worldCoords);
			F32 ndotwi = ei.rayInt.normal.v3Dot(wi);

			if (ndotwi > 0.0f)
			{
				Ray shadowRay(ei.rayInt.worldCoords, wi, RAY_EPSILON);
				ElementIntersection shadowEI;
				world.CheckRayElementIntersections(shadowEI, shadowRay);

				F32 distanceToLight = light->GetDistanceFromPoint(ei.rayInt.worldCoords);

				bool bNotInShadow = !shadowEI.element || (shadowEI.rayInt.t >= distanceToLight);
				if (bNotInShadow)
					color += diffuseBRDF.F(ei, wi, wo) * light->CalculateRadiance(ei) * ndotwi;
			}
		}

		return color;
	}
}