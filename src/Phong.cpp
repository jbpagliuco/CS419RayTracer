#include <Material.h>

#include <World.h>

namespace RE
{
	Phong::Phong(F32 ka, F32 kd, const Color& cd, F32 ks, const Color& cs, F32 exp) : 
		ambientBRDF(ka, cd), diffuseBRDF(kd, cd), specularBRDF(cs, ks, exp)
	{
	}

	Phong::~Phong()
	{

	}

	Color Phong::Shade(const ElementIntersection& ei, World& world)
	{
		VML::Vector wo = ei.ray.GetDirection().negate();

		// Add ambient light
		Color color = ambientBRDF.Rho(ei, wo) * world.ambientLight.CalculateRadiance(ei, world);

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
				bool bInShadow = light->InShadow(shadowRay, ei, world);

				if (!bInShadow)
				{
					Color f = diffuseBRDF.F(ei, wi, wo) + specularBRDF.F(ei, wi, wo);
					color += f * light->CalculateRadiance(ei, world) * ndotwi;
				}
			}
		}

		return color;
	}

	Color Phong::AreaLightShade(const ElementIntersection& ei, World& world)
	{
		VML::Vector wo = ei.ray.GetDirection().negate();

		// Add ambient light
		Color color = ambientBRDF.Rho(ei, wo) * world.ambientLight.CalculateRadiance(ei, world);

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
				bool bInShadow = light->InShadow(shadowRay, ei, world);

				if (!bInShadow)
				{
					Color f = diffuseBRDF.F(ei, wi, wo) + specularBRDF.F(ei, wi, wo);
					color += f * light->CalculateRadiance(ei, world) *
						light->G(ei) * ndotwi / light->PDF(ei);
				}
			}
		}

		return color;
	}
}