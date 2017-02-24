#include "Geometry.h"

namespace RE
{
	Sphere::Sphere() : radius(1.0f)
	{
	}

	Sphere::Sphere(F32 radius) : radius(radius)
	{
	}

	bool Sphere::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const
	{
		return BoundingSphere(elementTransform.position, radius).Intersects(outHitInfo, ray);
	}
}