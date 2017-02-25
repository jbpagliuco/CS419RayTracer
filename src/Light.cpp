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
}