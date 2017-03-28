#include <Light.h>

#include <World.h>
#include <algorithm>

namespace RE
{
	Light::Light(bool bCastsShadows) : bCastsShadows(bCastsShadows)
	{

	}

	Light::~Light()
	{

	}


	bool Light::InShadow(const Ray& ray, const ElementIntersection& ei, const World& world)const
	{
		if (!bCastsShadows)
			return false;

		F32 d = GetDistanceFromPoint(ray.GetOrigin());

		bool bHit;
		world.CheckRayElementIntersections(bHit, d, ray);

		return bHit;
	}


	F32 Light::G(const ElementIntersection& ei)const
	{
		return 0.0f;
	}

	F32 Light::PDF(const ElementIntersection& ei)const
	{
		return 0.0f;
	}
}